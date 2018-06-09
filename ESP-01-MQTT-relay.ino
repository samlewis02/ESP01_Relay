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
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "esp01-relay";
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// set up the feeds
AdafruitIO_Feed *esp_01 = io.feed("mainroom"); // subscribe toggle switch input ("ON"/"OFF")
#define RELAY_PIN 0
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
// static IP settings
IPAddress ip(192,168,1,130);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() {
pinMode(RELAY_PIN, OUTPUT);
digitalWrite(RELAY_PIN, LOW); // relay off
// start the serial connection
Serial.begin(115200);
WiFi.mode(WIFI_AP_STA);
// static IP
WiFi.config(ip,gateway,subnet);
WiFi.begin(ssid, password);

while(WiFi.waitForConnectResult() != WL_CONNECTED){
  WiFi.begin(ssid, password);
  Serial.println("WiFi failed, retrying.");
}

Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
MDNS.begin(host);

  httpUpdater.setup(&httpServer);
httpServer.begin();

MDNS.addService("http", "tcp", 80);
Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);


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
 httpServer.handleClient();
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
