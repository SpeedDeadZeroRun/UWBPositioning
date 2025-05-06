#include<SPI.h>
#include<DW1000Ranging.h>
#include"FunctionLibrary/link.h"
#include"FunctionLibrary/UWB_Math.h"
#include"FunctionLibrary/UWB_Matrix.h"
#include"FunctionLibrary/UWB_Newton.h"

#define ADDRESS "10:01:00:00:00:00:00:00"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define PIN_RST 27
#define PIN_IRQ 34

struct MyLink *uwb_data;
long runtime = 0;
float guess[3] = {0.0}; // initial guess

void newRange();
void newDevice(DW1000Device *device);
void inactiveDevice(DW1000Device *device);
void DW1000RangingLoop(void *);
void etc(void *);

#define anchor_count 4

// {X, Y, Z} units:milimeter
float anchor_location[4][3] = {
    {0, 0, 0}, // Anchor A
    {500, 866, 0}, // Anchor B
    {1000, 0, 0}, // Anchor C
    {500, 288.6758, 816.494}  // Anchor D
  };
char anchor_label[anchor_count] = {'A', 'B', 'C', 'D'} ;
uint16_t anchor_address[anchor_count] = {2560, 2816, 3072, 3328} ;

void setup() {
  Serial.begin(115200);
  delay(1000);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);
  DW1000.setAntennaDelay(16500);
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  DW1000Ranging.startAsTag(ADDRESS, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);

  uwb_data = init_link();

  xTaskCreatePinnedToCore(&DW1000RangingLoop, "DWLoop", 2*1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(&etc, "etc", 4*1024, NULL, 1, NULL, 1);
}

void loop() { vTaskDelete(NULL); }

void newRange() {
  uint16_t addr = DW1000Ranging.getDistantDevice()->getShortAddress();
  float range = DW1000Ranging.getDistantDevice()->getRange()*1000;
  float rssi = DW1000Ranging.getDistantDevice()->getRXPower();
  fresh_link(uwb_data, addr, range, rssi);
}

void newDevice(DW1000Device *device) {
  uint16_t addr = device->getShortAddress();
  if(find_link(uwb_data, addr) == NULL) {
    add_link(uwb_data, addr);
  }
}

void inactiveDevice(DW1000Device *device) {
  delete_link(uwb_data, device->getShortAddress());
}

void DW1000RangingLoop(void *) {
  while(1) {
    DW1000Ranging.loop();
  }
}

void etc(void *) {
  while(1) {
    if ((millis() - runtime) > 100) {
      long start_time = millis();
      float tag_distance[4] = {0.0};
      float tag_signal[4] = {0.0};
      for(int i = 0; i < 4; i++){
        struct MyLink *lk = find_link(uwb_data, anchor_address[i]);
        if(lk == NULL) continue;
        tag_distance[i] = get_avg_range(lk);
        tag_signal[i] = get_avg_rssi(lk);
        Serial.printf("Anchor %c: distance: %.2fmm rssi %.2fdBm\n", 
                      anchor_label[i], tag_distance[i], tag_signal[i]
                    );
      }

      for(int i = 0; i < 5; i++) {
        newton_3b4_3(guess, anchor_location, tag_distance);
      }
      
      Serial.printf("pos x y z:");
      for(int i = 0; i < 3; i++) {
        Serial.printf(" %.2f", guess[i]);
      }
      Serial.printf("\n\n");

      runtime = millis();
    }
  }
}
