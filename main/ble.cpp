#include <Arduino.h>

#include "ble.h"
#include "global.h"

#include <NimBLEDevice.h>

NimBLEScan* scan;
int thresholdBLE = 80;
char BEACON_ADRESS[18];
String BEACON_NAME;
std::vector<Beacon> beacons;
bool searching4BLE = false;
volatile int rssiBeacon = -10000;
volatile bool beaconTrobat = false;
unsigned long ultimPrint = 0;
class ScanCallbacks : public NimBLEScanCallbacks {

    void onResult(const NimBLEAdvertisedDevice* device) override {

        
        std::string mac = device->getAddress().toString();

        if (mac == BEACON_ADRESS) {

            rssiBeacon = device->getRSSI();
            beaconTrobat = true;

        }

        bool tempb = false;
        for (auto &b : beacons) {
            if (b.mac == mac) {
                tempb = true;
                break;
            }
        }
        if (searching4BLE && !tempb) {
            beacons.push_back({
                device->getName(),
                mac
            });
        }
    }
};

void setup_ble(){

  NimBLEDevice::init("");

  scan = NimBLEDevice::getScan();

  scan->setScanCallbacks(new ScanCallbacks());

  scan->setActiveScan(true);

  scan->setInterval(100);
  scan->setWindow(100);

  scan->setDuplicateFilter(false);

}

void update_ble(){

  if (!scan->isScanning() && millis() - ultimPrint > 1000) {
        
        scan->start(250, false);

        Serial.print("RSSI beacon " + BEACON_NAME + ": ");
        Serial.println(rssiBeacon);


        ultimPrint = millis();
        
        
    }else if (millis() - ultimPrint >= 3000){
        scan->stop();
    }

    if (rssiBeacon > -thresholdBLE && !armed){
        arm();
    }

}