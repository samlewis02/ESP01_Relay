/*
 * ESP-01S
 * 
 * Turn relay on and off using the mainroom feed from AdafruitIO
 * 
 * 26/5/18
 * 
 */
#include "MySSID.h"
#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// set up the feeds
AdafruitIO_Feed *esp_01 = io.feed("mainroom"); // subscribe toggle switch input ("ON"/"OFF")
#define RELAY_PIN 0

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // relay off
  // start the serial connection
  Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.println("Connecting to Adafruit IO");
  io.connect();

  esp_01->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  io.run();
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("esp_01 received <- ");

  if (data->toString() == "ON") {
    Serial.println("Relay ON");
    digitalWrite(RELAY_PIN, HIGH);
  }
  else if (data->toString() == "OFF") {
    Serial.println("Relay OFF");
    digitalWrite(RELAY_PIN, LOW);

  }
}

