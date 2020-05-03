#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

const int vibrationPin = A0;
int sensorValue = 0;

const int wifiButton = 16;
int buttonPressed = 0;

const int webhookButton = 0;
int webbuttonPressed = 0;
const char* host = "maker.IFTTT.com";

void wifisetup() {
  Serial.begin(115200);
  Serial.println();
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  wifiManager.resetSettings();
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected...yay :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected...yay :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  pinMode(wifiButton, INPUT);

}
void loop() {
  buttonPressed = digitalRead(wifiButton);
  if (buttonPressed == LOW) {
    Serial.println();
    Serial.println("FORGETTING WIFI NETWORK. YOU NEED TO RECONFIGURE WIFI!");
    delay(3000);
    wifisetup();
    delay(5000);
  }

  webbuttonPressed = digitalRead(webhookButton);
  if (webbuttonPressed == LOW) {
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    
    String url = "https://maker.ifttt.com/trigger/esp8266_test/with/key/d2wy4sRGiMLi3iboCcOgva";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);
    Serial.println("Respond:");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println("closing connection");
    delay(5000);
  }


  sensorValue = analogRead(vibrationPin);
  if (sensorValue > 1023) {
    Serial.println("Vibration detected");
    delay(200);
  }
}
