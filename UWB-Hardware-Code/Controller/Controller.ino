#include<Arduino.h>
#include<ArduinoJson.h>
#include<HTTPClient.h>
#include<WiFi.h>
#include<WiFiClientSecure.h>
#include"Secret.h"
#include"Certificate.h"
#include"tagList.h"
#include"FunctionLibrary/parser.h"

#define PIN_SEL1 0
#define PIN_SEL2 1
#define PIN_SEL3 10
#define PIN_SEL4 11
#define PIN_DEBUG LED_BUILTIN
struct AnchorStruct *anchors[4];
struct Data *tagData;

const String SERVER = "https://project.your_domain.xyz/api/";
String APIToken = "";

void connectWifi();
void anchorInit(struct AnchorStruct *anchor);
int login();
String sendCommand(int selPin, String command);
String getMeasureLogs();
String payloadBuilder(String tagLists);
int addMeasureLogs(String payload);
unsigned long lastLogs = 0;

struct AnchorStruct {
    int PIN_SEL;
    String name;
    String ip;
};

struct AnchorStruct *newAnchor(int sel, String n, String i) {
    struct AnchorStruct *a = (struct AnchorStruct *)malloc(sizeof(struct AnchorStruct));
    a->PIN_SEL = sel;
    a->name = n;
    a->ip = i;
    return a;
}

void setup() {
  pinMode(PIN_SEL1, OUTPUT);
  pinMode(PIN_SEL2, OUTPUT);
  pinMode(PIN_SEL3, OUTPUT);
  pinMode(PIN_SEL4, OUTPUT);
  pinMode(PIN_DEBUG, OUTPUT);
  digitalWrite(PIN_SEL1, HIGH);
  digitalWrite(PIN_SEL2, HIGH);
  digitalWrite(PIN_SEL3, HIGH);
  digitalWrite(PIN_SEL4, HIGH);
  digitalWrite(PIN_DEBUG, LOW);
  anchors[0] = newAnchor(PIN_SEL1, "A", "00:0A");
  anchors[1] = newAnchor(PIN_SEL2, "B", "00:0B");
  anchors[2] = newAnchor(PIN_SEL3, "C", "00:0C");
  anchors[3] = newAnchor(PIN_SEL4, "D", "00:0D");
  Serial.begin(115200);
  connectWifi();
  if(WiFi.status() == WL_CONNECTED) login();
  delay(1000);
  while(Serial.available()) Serial.read();
  for(int i = 0; i < 4; i++) {
    anchorInit(anchors[i]);
  }
  tagData =  init_data();
  digitalWrite(PIN_DEBUG, HIGH);
}

void loop() {
  if((WiFi.status() == WL_CONNECTED) &&
    ((millis() - lastLogs) > 3000)
  ) {
    if(APIToken == "") login();
    else {
      String tagLists = getMeasureLogs();
      if(tagLists != "") {
        String payload = payloadBuilder(tagLists);
        int httpCode = addMeasureLogs(payload);
      }
    }
    lastLogs = millis();
  }
}

void anchorInit(struct AnchorStruct *anchor) {
  int PIN_SEL = anchor->PIN_SEL;
  String name = anchor->name;
  String ip = anchor->ip;
  sendCommand(PIN_SEL, "sel set led on");
  sendCommand(PIN_SEL, "sel set name "+name);
  sendCommand(PIN_SEL, name+" set ip "+ip);
  sendCommand(PIN_SEL, name+" set start");
  sendCommand(PIN_SEL, name+" set led off");
}

String sendCommand(int selPin, String command) {
  digitalWrite(selPin, LOW);
  String serialData = "";
  while(Serial.available()) Serial.read();
  Serial.print(command);
  Serial.print("\n");
  unsigned long lastRecive = millis();
  while(true) {
    if(Serial.available()) {
      char dataChar = Serial.read();
      lastRecive = millis();
      if(dataChar == '\n') break;
      if(isAscii(dataChar) && (dataChar != '\0')) {
        serialData = serialData+dataChar;
      }
    }
    if((millis() - lastRecive) > 500) {
      break;
    }
  }
  digitalWrite(selPin, HIGH);

  return serialData;
}

String getMeasureLogs() {
  String tagLists = "";
  for(int i = 0; i < 4; i++) {
    int PIN_SEL = anchors[i]->PIN_SEL;
    String name = anchors[i]->name;
    sendCommand(PIN_SEL, name+" set led on");
    String serialData = sendCommand(PIN_SEL, name+" get tag all");
    sendCommand(PIN_SEL, name+" set led off");
    if(serialData != "") {
      int head = 0;
      int tail = 0;
      String temp = "";
      while(tail < serialData.length()) {
          if(serialData[tail] == ',') {
              temp = temp+anchors[i]->ip+" "+serialData.substring(head, tail+1);
              head = tail+1;
          }
          tail = tail+1;
      }
      temp = temp+anchors[i]->ip+" "+serialData.substring(head, serialData.length());
      tagLists = tagLists+temp+",";
    }
  }

  return tagLists;
}

String payloadBuilder(String tagLists) {
  int curser = 0;
  JsonDocument doc;
  doc["Status"] = "ok";
  for(int i = 0; curser != -1; i++) {
    curser = parse(tagData, tagLists.c_str(), curser, tagLists.length());
    doc["data"][i]["timestamp"] = "";
    doc["data"][i]["anchor"] = tagData->anchor_ip;
    doc["data"][i]["tag"] = tagData->tag_ip;
    doc["data"][i]["distance"] = tagData->distance;
    doc["data"][i]["rssi"] = tagData->rssi;
  }
  String returnString = "";
  serializeJson(doc, returnString);
  
  return returnString;
}

int addMeasureLogs(String payload) {
  String path = SERVER + "AddMeasureLogs";
  HTTPClient https;
  https.begin(path, rootCACertificate);
  https.addHeader("Authorization",  "Bearer " + APIToken);
  https.addHeader("Content-Type", "application/json");
  int httpCode = https.POST(payload);
  https.end();

  return httpCode;
}

void connectWifi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  int debugLight = 0;
  unsigned long waitSince = millis();
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(PIN_DEBUG, debugLight);
    debugLight = !debugLight;
    delay(500);
    if((millis() - waitSince) > 10000) break;
  }
  if(WiFi.status() == WL_CONNECTED) digitalWrite(PIN_DEBUG, HIGH);
  else digitalWrite(PIN_DEBUG, LOW);
}

int login() {
  APIToken = "";
  String path = SERVER + "login?name=" + name + "&secret=" + secret;
  HTTPClient https;
  https.begin(path, rootCACertificate);

  int httpCode = https.POST("");
  if (httpCode == 200) {
    String response = https.getString();
    JsonDocument doc;
    deserializeJson(doc, response);
    APIToken = doc["token"].as<String>();
  }
  https.end();

  return httpCode;
}
