#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <NimBLEDevice.h>


// SERVER
const char* ssid = "Target";
const char* password = "12345678";

// SERVO
Servo servo;
const int servoPin = 18;

// PIEZO
const int piezoPin = 34;
int thresholdPiezo = 100;

// BLE
NimBLEScan* scan;
int thresholdBLE = 80;
#define BEACON_ADDRESS "fe:dc:d8:11:14:93"
volatile int rssiBeacon = -10000;
volatile bool beaconTrobat = false;
class ScanCallbacks : public NimBLEScanCallbacks {

    void onResult(const NimBLEAdvertisedDevice* device) override {

        if (device->getAddress().toString() == BEACON_ADDRESS) {

            rssiBeacon = device->getRSSI();
            beaconTrobat = true;

        }
    }
};

// RADAR
bool radarPresence = false;

//WEB
DNSServer dnsServer;
WebServer server(80);

const byte DNS_PORT = 53;

const char pagina[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Target</title>
</head>

<body>

<h1>Target</h1>

<button onclick="fetch('/arm')">
Armar
</button>
<button onclick="fetch('/disarm')">
Desarmar
</button>

<hr>

<h2>Temporizar</h2>

Esperar <input type="text" id="before" value="5">s<br>
Armar durante <input type="text" id="while" value="5">s<br>

<button onclick="timed()">
Run
</button>

<script>
function timed(){
    let bval = document.getElementById("before").value;
    let wval = document.getElementById("while").value;
    fetch("/timed?bval=" + bval + "&wval=" + wval);
}
</script>

<hr>

<h2>Variar sensibilidad piezo</h2>

Threshold: <input type="text" id="thresholdPiezo" value="100">

<button onclick="updateThresholdPiezo()">
Update
</button>

<script>
function updateThresholdPiezo(){
    let val = document.getElementById("thresholdPiezo").value;
    fetch("/updateThresholdPiezo?value=" + val);
}
</script>

<hr>

<h2>Variar sensibilidad BLE</h2>

Threshold: <input type="text" id="thresholdBLE" value="80">

<button onclick="updateThresholdBLE()">
Update
</button>

<script>
function updateThresholdBLE(){
    let val = document.getElementById("thresholdBLE").value;
    fetch("/updateThresholdBLE?value=" + val);
}
</script>

</body>
</html>
)rawliteral";



bool armed = false;

bool timedRunning;
bool timedArmed;

unsigned long startTime;
unsigned long beforeTime;
unsigned long armedTime;

void arm() {
    servo.write(90);
    armed = true;
}
void disarm() {
    servo.write(0);
    armed = false;
}
void timed(int b, int w) {
    delay(b);
    arm();
    delay(w);
    disarm();
}



void setup() {
  Serial.begin(115200);
  // WIFI

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // SERVO
  servo.attach(servoPin);

  // WEB
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", []() {
      server.send(200, "text/html", pagina);
  });

  server.on("/arm", []() {

      arm();

      server.send(200, "text/plain", "OK");

      armed = true;
  });

  server.on("/disarm", []() {

      disarm();

      server.send(200, "text/plain", "OK");

      armed = false;
  });

  server.on("/updateThresholdPiezo", []() {

        String valor = server.arg("value");
        thresholdPiezo = valor.toInt();

        server.send(200, "text/plain", "OK");

  });
  server.on("/updateThresholdBLE", []() {

        String valor = server.arg("value");
        thresholdBLE = valor.toInt();

        server.send(200, "text/plain", "OK");

  });
  server.on("/presence", HTTP_POST, [](){

    if (server.hasArg("presence")) {

      radarPresence = server.arg("presence") == "1";
      Serial.print("Radar: ");
      Serial.println(radarPresence);

    }

    server.send(200, "text/plain", "OK");
  });

  server.on("/timed", []() {

    beforeTime = server.arg("bval").toInt() * 1000;
    armedTime  = server.arg("wval").toInt() * 1000;

    startTime = millis();
    timedRunning = true;
    timedArmed = false;

    server.send(200, "text/plain", "OK");
});

  server.onNotFound([]() {
      server.send(200, "text/html", pagina);
  });

  
  // BLE
  NimBLEDevice::init("");

  scan = NimBLEDevice::getScan();

  scan->setScanCallbacks(new ScanCallbacks());

  scan->setActiveScan(true);

  scan->setInterval(100);
  scan->setWindow(100);

  scan->setDuplicateFilter(false);

  // SETUP 
  server.begin();
  disarm();
  
}


unsigned long ultimPrint = 0;

void loop() {

    if (!scan->isScanning() && millis() - ultimPrint > 1000) {
        
        scan->start(200, false);

        Serial.print("RSSI beacon: ");
        Serial.println(rssiBeacon);


        ultimPrint = millis();
        
        
    }else if (millis() - ultimPrint >= 3000){
        scan->stop();
    }

    dnsServer.processNextRequest();
    server.handleClient();
    if (timedRunning) {

        unsigned long elapsed = millis() - startTime;

        if (!timedArmed && elapsed >= beforeTime) {
            arm();
            timedArmed = true;
        }

        if (timedArmed && elapsed >= beforeTime + armedTime) {
            disarm();
            timedRunning = false;
        }
    }
    if (analogRead(piezoPin) > thresholdPiezo && armed){
        disarm();
    }
    if (rssiBeacon > -thresholdBLE && !armed){
        arm();
    }
    if(radarPresence){
        arm();
    }else{
        disarm();
    }
    
}