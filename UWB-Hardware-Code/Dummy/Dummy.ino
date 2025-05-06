#include <SPI.h>
#include "DW1000Ranging.h"
 
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define PIN_SS 4
#define PIN_RST 27
#define PIN_IRQ 34

char UWBIps[] = "10:02:00:00:00:00:00:00";
 
void setup() {
    delay(1000);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    DW1000.setAntennaDelay(16500);
    DW1000Ranging.startAsTag(UWBIps, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}
 
void loop() {
    DW1000Ranging.loop();
}
 
void newRange() {
}
 
void newDevice(DW1000Device *device) {
}
 
void inactiveDevice(DW1000Device *device) {
}
