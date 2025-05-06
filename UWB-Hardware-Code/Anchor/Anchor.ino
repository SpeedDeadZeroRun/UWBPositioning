#include<SPI.h>
#include"DW1000Ranging.h"
#include"FunctionLibrary/link.h"
#include"FunctionLibrary/parser.h"

// For green Board
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define PIN_SS 5
#define PIN_RST 27
#define PIN_IRQ 35

// For Red Board
// #define SPI_SCK 18
// #define SPI_MISO 19
// #define SPI_MOSI 23
// #define PIN_SS 4
// #define PIN_RST 27
// #define PIN_IRQ 34

#define PIN_DEBUG 17
#define PIN_SELECT 21
#define PIN_0 0
#define PIN_TX 1
#define PIN_RX 3

struct MyLink *uwb_data;
struct Command *c;
String serialData;
int isSelect = 0;
char anchorName[24] = "anchor";
char UWBIp[] = "00:00:00:00:00:00:00:00";

unsigned long lastFetch = 0;

void newRange();
void newBlink(DW1000Device *device);
void inactiveDevice(DW1000Device *device);
void DW1000RangingLoop(void *);
void serialProcess();
void commandProcess();
void setProcess();
void getProcess();
void getTagProcess();
void setDebugLED();
void fetchLink();
void etc(void *);
bool startable = false;

void setup() {
  pinMode(PIN_DEBUG, OUTPUT);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  digitalWrite(PIN_DEBUG, LOW);
  Serial.begin(115200, SERIAL_8N1, PIN_RX, PIN_0);
  pinMode(PIN_TX, INPUT);
  c = init_command();
  uwb_data = init_link();
  delay(1000);
  Serial.flush();
  while(Serial.available()) Serial.read();
  while(!startable) serialProcess();
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); 
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  DW1000Ranging.startAsAnchor(UWBIp, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);

  xTaskCreatePinnedToCore(&DW1000RangingLoop, "DWLoop", 2*1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(&etc, "etc", 4*1024, NULL, 1, NULL, 1);
}

void loop() { vTaskDelete(NULL); }

void DW1000RangingLoop(void *) {
  while(1) {
    DW1000Ranging.loop();
  }
}

void etc(void *) {
  while(1) {
    serialProcess();
    if((millis()-lastFetch) > 3000) fetchLink();
  }
}

void newRange() {
  uint16_t addr = DW1000Ranging.getDistantDevice()->getShortAddress();
  float range = DW1000Ranging.getDistantDevice()->getRange()*1000;
  float rssi = DW1000Ranging.getDistantDevice()->getRXPower();
  fresh_link(uwb_data, addr, range, rssi);
}

void newBlink(DW1000Device *device) {
  uint16_t addr = device->getShortAddress();
  if(find_link(uwb_data, addr) == NULL) {
    add_link(uwb_data, addr);
  }
  else {
    newRange();
  }
}

void inactiveDevice(DW1000Device *device) {
  delete_link(uwb_data, device->getShortAddress());
}

void serialProcess() {
  while(Serial.available()) {
    char dataChar = Serial.read();
    serialData = serialData+dataChar;
    if(dataChar == '\n') {
      commandProcess();
    }
  }
}

void commandProcess() {
  isSelect = !digitalRead(PIN_SELECT);
  parse(c, serialData.c_str(), serialData.length());
  serialData = "";
  if(
      !strcmp(c->sel, "all") ||
      !strcmp(c->sel, anchorName) ||
      (!strcmp(c->sel, "sel") && isSelect)
  ) {
    Serial.flush();
    Serial.setPins(PIN_RX, PIN_TX);
    if(!strcmp(c->arg1, "set")) setProcess();
    else if(!strcmp(c->arg1, "get")) getProcess();
    Serial.print("\n");
    Serial.flush();
    Serial.setPins(PIN_RX, PIN_0);
    pinMode(PIN_TX, INPUT);
  }
  isSelect = 0;
}

void setProcess() {
  if(!strcmp(c->arg2, "name")) strncpy(anchorName, c->arg3, 8);
  else if(!strcmp(c->arg2, "ip")) strncpy(UWBIp, c->arg3, 5);
  else if(!strcmp(c->arg2, "start")) startable = true;
  else if(!strcmp(c->arg2, "led")) setDebugLED();
}

void getProcess() {
  if(!strcmp(c->arg2, "name")) Serial.print(anchorName);
  else if(!strcmp(c->arg2, "ip")) Serial.print(UWBIp);
  else if(!strcmp(c->arg2, "tag")) getTagProcess();
}

void getTagProcess() {
  if(uwb_data == NULL) return;
  else if(!strcmp(c->arg3, "all")) {
    struct MyLink *tag_data = uwb_data->next;
    while(tag_data != NULL) {
      serialPrintTag(tag_data);
      if(tag_data->next != NULL) Serial.print(",");
      tag_data = tag_data->next;
    }
  }
}

void setDebugLED() {
  if(!strcmp(c->arg3, "on")) digitalWrite(PIN_DEBUG, HIGH);
  else if(!strcmp(c->arg3, "off")) digitalWrite(PIN_DEBUG, LOW);
}

void serialPrintTag(struct MyLink *tag_data) {
  String anchorIp = "";
  anchorIp = anchorIp+UWBIp[0];
  anchorIp = anchorIp+UWBIp[1];
  anchorIp = anchorIp+":";
  anchorIp = anchorIp+UWBIp[3];
  anchorIp = anchorIp+UWBIp[4];
  String temp = String(tag_data->address, HEX);
  for(int i = temp.length(); i < 4; i++) {
    temp = "0"+temp;
  }
  String tagIp = "";
  tagIp = tagIp+temp[2];
  tagIp = tagIp+temp[3];
  tagIp = tagIp+":";
  tagIp = tagIp+temp[0];
  tagIp = tagIp+temp[1];
  Serial.printf("%s ", tagIp);
  Serial.printf("%.0f ", get_avg_range(tag_data));
  Serial.printf("%.2f", get_avg_rssi(tag_data));
}

void fetchLink() {
  if(uwb_data == NULL) return;
  
  struct MyLink *tag_data = uwb_data->next;
  while(tag_data != NULL) {
    if((millis()-(tag_data->last_seen)) > 5000) {
      delete_link(uwb_data, tag_data->address);
    }
    tag_data = tag_data->next;
  }
  lastFetch = millis();
}
