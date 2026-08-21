#ifndef BLE_H
#define BLE_H

#include <string>

void setup_ble();
void update_ble();

struct Beacon {
    std::string name;
    std::string mac;
};
extern std::vector<Beacon> beacons;
extern char BEACON_ADRESS[18];
extern String BEACON_NAME;
extern bool searching4BLE;
extern int thresholdBLE;
extern bool toggleBLE;

#endif