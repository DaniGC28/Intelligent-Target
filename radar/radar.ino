#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Target";
const char* password = "12345678";

const char* serverURL = "http://192.168.4.1/presence";

const int pinRadar = 27;
bool radarPresence = false;

void setup() {
  Serial.begin(115200);
  pinMode(pinRadar, INPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connectant...");
  }

  Serial.println("Connectat!");
  Serial.println(WiFi.localIP());

}

void loop() {
  //Serial.println(digitalRead(pinRadar));
  if (radarPresence != digitalRead(pinRadar)){
    radarPresence = digitalRead(pinRadar);

    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      http.begin(serverURL);

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String data = "presence=" + String(radarPresence);

      int response = http.POST(data);

      Serial.print("Resposta servidor: ");
      Serial.println(response);

      http.end();

  }
  }
  delay(100);
}