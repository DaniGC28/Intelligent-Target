#include <Arduino.h>

#include "web.h"
#include "global.h"
#include "piezo.h"
#include "ble.h"
#include "radar.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

const char* ssid = "Target";
const char* password = "12345678";

DNSServer dnsServer;
WebServer server(80);

const byte DNS_PORT = 53;

void setup_web(){

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    server.on("/", []() {
        server.send(200, "text/html", html);
    });

    server.on("/getPos", []() {
      JsonDocument doc;

      doc["left"] = lPos;
      doc["right"] = rPos;

      String resposta;
      serializeJson(doc, resposta);

      server.send(200, "application/json", resposta);
    });
    server.on("/getTogglePiezo", []() {
      server.send(200, "text/plain", togglePiezo);
    });
    server.on("/getToggleRadar", []() {
      server.send(200, "text/plain", toggleRadar);
    });
    server.on("/getToggleBLE", []() {
      server.send(200, "text/plain", toggleBLE);
    });

    server.on("/updateRightPos", []() {
      rPos = server.arg("pos").toInt();

      server.send(200, "text/plain", "OK");
    });
    server.on("/updateLeftPos", []() {
      lPos = server.arg("pos").toInt();

      server.send(200, "text/plain", "OK");
    });

    server.on("/arm", []() {

        String valuePos = server.arg("pos");

        pos = valuePos.toInt();

        arm(calibration, pos);

        server.send(200, "text/plain", "OK");

        armed = true;
    });
    server.on("/disarm", []() {

        disarm();

        server.send(200, "text/plain", "OK");

        armed = false;
    });

    server.on("/timed", []() {

      beforeTime = server.arg("bval").toInt() * 1000;
      armedTime  = server.arg("wval").toInt() * 1000;
      timedPos = server.arg("pos");

      startTime = millis();
      timedRunning = true;
      timedArmed = false;

      server.send(200, "text/plain", "OK");
  });
    server.on("/stopTimed", []() {

      if (timedRunning){
        timedRunning = false;
      }
      if (timedArmed){
        timedArmed = false;
        disarm();
      }
      
    });
    server.on("/timedStatus", []() {
      server.send(200, "text/plain", timedRunning ? "1" : "0");
    });

    server.on("/togglePiezo", []() {
      bool val = server.arg("value") == "true";
      togglePiezo = val;
      
      server.send(200, "text/plain", "OK");
    });
    server.on("/updateThresholdPiezo", []() {

          String valor = server.arg("value");
          thresholdPiezo = valor.toInt();

          server.send(200, "text/plain", "OK");

    });

    server.on("/toggleRadar", []() {
      bool val = server.arg("value") == "true";
      toggleRadar = val;
      
      server.send(200, "text/plain", "OK");
    });

    server.on("/toggleBLE", []() {
      bool val = server.arg("value") == "true";
      toggleBLE = val;
      
      server.send(200, "text/plain", "OK");
    });
    server.on("/updateThresholdBLE", []() {

          String valor = server.arg("value");
          thresholdBLE = valor.toInt();

          server.send(200, "text/plain", "OK");

    });
    server.on("/startSearch", []() {

          beacons.clear();
          searching4BLE = true;

          server.send(200, "text/plain", "OK");

    });
    server.on("/stopSearch", []() {

          searching4BLE = false;

          delay(500);

          String resposta = BEACON_NAME + " con mac " + BEACON_ADRESS;

          server.send(200, "text/plain", resposta);

    });
    server.on("/getBeacons", HTTP_GET, [](){

      JsonDocument doc;
      JsonArray array = doc.to<JsonArray>();

      for(auto &b : beacons){
          JsonObject obj = array.add<JsonObject>();

          obj["name"] = b.name;
          obj["mac"] = b.mac;
      }
          String resposta;
          serializeJson(doc, resposta);

          server.send(200, "application/json", resposta);
      });
    server.on("/BLEselected", []() {

          server.arg("mac").toCharArray(BEACON_ADRESS, sizeof(BEACON_ADRESS));
          for(auto &b : beacons){
              if (b.mac == BEACON_ADRESS){
                BEACON_NAME = String(b.name.c_str());
              }
          }

          

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


    server.on("/script.js", []() {
      server.send(200, "application/javascript", js);
    });
    server.on("/style.css", []() {
      server.send(200, "text/css", css);
    });

    server.onNotFound([]() {
        server.send(200, "text/html", html);
    });


  server.begin();
}

void update_web(){

  dnsServer.processNextRequest();
  server.handleClient();

}