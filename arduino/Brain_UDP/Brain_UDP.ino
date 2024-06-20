// Get brain data over UDP
#include <Brain.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* SSID = "SSID";
const char* pass = "PASS";
bool transmit;
unsigned int port = 4120;
char incomingPacket[255]; //Packet buffer
WiFiUDP Udp;

Brain brain(Serial);

void setup() {
    Serial.begin(9600);
    Serial.println();

    pinMode(LED_BUILTIN, OUTPUT); //digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(LED_BUILTIN, LOW);

    Serial.printf("Connecting to %s", SSID);
    WiFi.begin(SSID, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("connected.");
    for (int i = 0; i <= 8; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(35);
      digitalWrite(LED_BUILTIN, LOW);
      delay(35);
    }
    Udp.begin(port);
    Serial.printf("Running at IP %s, UDP port %d - ready to transmit.\n", WiFi.localIP().toString().c_str(), port);
}

void loop() {
    if (transmit) {
      if (brain.update()) {
        digitalWrite(LED_BUILTIN, LOW);
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(brain.readCSV());
        Serial.print("Attempting to transmit: ");
        Serial.println(brain.readCSV());
        Udp.endPacket();
        delay(300);
        int packetSize = Udp.parsePacket();
        if (!packetSize)
        {
            Serial.println("Didn't get response! Stopping transmission.");
            transmit = false;
            for (int i = 0; i <= 5; i++) {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(50);
                digitalWrite(LED_BUILTIN, LOW);
                delay(50);
            }
        }
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    else {
      int packetSize = Udp.parsePacket();
      if (packetSize) {
        Serial.println("Got packet. Starting to transmit.");
        transmit = true;
        for (int i = 0; i <= 2; i++) {
          digitalWrite(LED_BUILTIN, HIGH);
          delay(100);
          digitalWrite(LED_BUILTIN, LOW);
          delay(100);
        }
      }
    }
    //if (brain.update()) {
    //    Serial.println(brain.readErrors());
    //    Serial.println(brain.readCSV());
    //}
}
