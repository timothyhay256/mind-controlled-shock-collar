/*
This file is depreceated in favor of collar_rf
I wish I didn't spend so much time trying to reverse engineer the collar I had on hand lol
*/


#include <ESP8266WiFi.h> // This really needs cleaning
#include <WiFiUdp.h>

const char* SSID = "SSID";
const char* pass = "PASS";
unsigned int port = 4120; 
char incomingPacket[4]; //Packet buffer size.
WiFiUDP Udp;

// DEBUGGING PURPOSES
int beepDat[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}; //Temporary! Ideally, these can be gen on the fly.
int sizeBeepDat = sizeof(beepDat) / sizeof(beepDat[0]);
int shockLvl4[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };
int sizeShockLvl4 = sizeof(shockLvl4) / sizeof(shockLvl4[0]);

// 6 TIER
int shock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
int sizeShock = sizeof(shock) / sizeof(shock[0]);

int kiloShock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0 };
int sizeKiloShock = sizeof(kiloShock) / sizeof(kiloShock[0]);

int megaShock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0 };
int sizeMegaShock = sizeof(megaShock) / sizeof(megaShock[0]);

int gigaShock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0 };
int sizeGigaShock = sizeof(gigaShock) / sizeof(gigaShock[0]);

int teraShock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0 };
int sizeTeraShock = sizeof(teraShock) / sizeof(teraShock[0]);

int petaShock[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, };
int sizePetaShock = sizeof(petaShock) / sizeof(petaShock[0]);
int incomingByte = 0;
#define COLLAR_IN_PIN D2
// #define LED_BUILTIN 2 // ESP32

void triggerOn(int* dat, int size, int wait = 270) {
  // digitalWrite(LED_BUILTIN, HIGH);
  // int32_t btime, etime;
  // btime = asm_ccount();
  for (int i = 0; i < 500; i++) {
    digitalWrite(COLLAR_IN_PIN, random(2)); // Generate either 0 or 1
    delayMicroseconds(270);    
  }
  for (int i = 0; i < size; i++) {
    //Serial.print(dat[i]);
    digitalWrite(COLLAR_IN_PIN, dat[i]);

    // if (dat[i] == 1) {
    //   GPOS = (1 << COLLAR_IN_PIN);      
    // } else {
    //   GPOC = (1 << COLLAR_IN_PIN); 
    // }  
    // delayMicroseconds(wait-((uint32_t)(etime - btime)/80000));
    delayMicroseconds(wait);
  }
  // etime = asm_ccount();
  // Serial.println((uint32_t)(etime - btime));  
  digitalWrite(COLLAR_IN_PIN, 0); // Set data pin to low after sequence, collar expects this.
  // digitalWrite(LED_BUILTIN, LOW);
  for (int i = 0; i < 500; i++) {
    digitalWrite(COLLAR_IN_PIN, random(2)); // Generate either 0 or 1
    delayMicroseconds(270);    
  }
}

static inline int32_t asm_ccount(void) {

    int32_t r;

    asm volatile ("rsr %0, ccount" : "=r"(r));
    return r;
}

static uint32_t
__measure(void (*callback)()) {

    int32_t btime, etime;

    btime = asm_ccount();
    callback();
    etime = asm_ccount();
    return ((uint32_t)(etime - btime));
}

void setup() {
  Serial.begin(115200);
  pinMode(COLLAR_IN_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // // wakey wakey, here's some garbage data!
  // Serial.println("Feeding garbage to awake device");
  // for (int i = 0; i < 8000; i++) {
  //   digitalWrite(COLLAR_IN_PIN, random(2)); // Generate either 0 or 1
  //   delayMicroseconds(270);    
  // }
  // Serial.println("Finished feeding garbage, ready for input.");
  Serial.printf("Connecting to %s", SSID);
  digitalWrite(LED_BUILTIN, LOW);
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
    Serial.printf("Running at IP %s, UDP port %d - listening.\n", WiFi.localIP().toString().c_str(), port);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (Serial.available() > 0) { // Used for debugging.
    Serial.println("Got serial input.");
    // for (int i = 0; i <= 3; i++) { // Visually indicate received packet
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     delay(20);
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(20);
    // }
    incomingByte = Serial.read() - '0';
    digitalWrite(COLLAR_IN_PIN, incomingByte);
    if (incomingByte == 1) {
      digitalWrite(COLLAR_IN_PIN, HIGH);
    }
    if (incomingByte == 0) {
      digitalWrite(COLLAR_IN_PIN, LOW);
    }
    if (incomingByte == 2 ) {
      Serial.println("Running triggerOn");
      triggerOn(shockLvl4, sizeShockLvl4);
    }
    if (incomingByte == 3) {
      Serial.println("Feeding 5 kbits of garbage");
      for (int i = 0; i < 5000; i++) {
        digitalWrite(COLLAR_IN_PIN, random(2)); // Generate either 0 or 1
        delayMicroseconds(270);    
      }
      Serial.println("Finished feeding garbage");
    }
    if (incomingByte == 4) {
      triggerOn(shock, sizeShock);
    }
    if (incomingByte == 5) {
      triggerOn(kiloShock, sizeKiloShock);
    }
    if (incomingByte == 6) {
      triggerOn(megaShock, sizeMegaShock);
    }
    if (incomingByte == 7) {
      triggerOn(gigaShock, sizeGigaShock);
    }
    if (incomingByte == 8) {
      triggerOn(teraShock, sizeTeraShock);
    }
    if (incomingByte == 9) {
      triggerOn(petaShock, sizePetaShock);
    }
    delayMicroseconds(270);
  } else if (packetSize) {
    int len = Udp.read(incomingPacket, 4);
    if (len > 0)
    {
      incomingPacket[len] = '\0';
    }
    // Serial.println("Got packet. Packet received:");
    Serial.println(incomingPacket);
    for (int i = 0; i <= 3; i++) { // Visually indicate received packet
        digitalWrite(LED_BUILTIN, HIGH);
        delay(20);
        digitalWrite(LED_BUILTIN, LOW);
        delay(20);
    }
    if (incomingPacket[0] == '1') {
        Serial.println("Beep received. Injecting beep...");
        triggerOn(beepDat, sizeBeepDat);
    } else if (incomingPacket[0] == '0') {
      Serial.println("Shock received, injecting shock at specified power...");
      if (incomingPacket[1] == '1') {
        // Serial.println("Injecting shock at power 1!");
        // Serial.println(__measure(triggerOn(shock, sizeShock));
    //             __measure(triggerOn(shock, sizeShock);
        // int32_t btime, etime;
        // btime = asm_ccount();
        triggerOn(shock, sizeShock);
        triggerOn(shock, sizeShock);
        triggerOn(shock, sizeShock);
        // for (int i = 0; i < 500; i++) {
        //   digitalWrite(COLLAR_IN_PIN, random(2)); // Generate either 0 or 1
        //   delayMicroseconds(270);    
        // }
        // triggerOn(shock, sizeShock);
        // etime = asm_ccount();
        // Serial.println((uint32_t)(etime - btime));  
      } else if (incomingPacket[1] == '2') {
        Serial.println("Injecting shock at power 2!");
        triggerOn(kiloShock, sizeKiloShock);
        triggerOn(kiloShock, sizeKiloShock);
        triggerOn(kiloShock, sizeKiloShock);
      } else if (incomingPacket[1] == '3') {
        Serial.println("Injecting shock at power 3!");
        triggerOn(megaShock, sizeMegaShock);
        triggerOn(megaShock, sizeMegaShock);
        triggerOn(megaShock, sizeMegaShock);
      } else if (incomingPacket[1] == '4') {
        Serial.println("Injecting shock at power 4!");
        triggerOn(gigaShock, sizeGigaShock);
        triggerOn(gigaShock, sizeGigaShock);
        triggerOn(gigaShock, sizeGigaShock);
      } else if (incomingPacket[1] == '5') {
        Serial.println("Injecting shock at power 5!");
        triggerOn(teraShock, sizeTeraShock);
        triggerOn(teraShock, sizeTeraShock);
        triggerOn(teraShock, sizeTeraShock);
      } else if (incomingPacket[1] == '6') {
        Serial.println("Injecting shock at power 6!");
        triggerOn(petaShock, sizePetaShock);
        triggerOn(petaShock, sizePetaShock);
        triggerOn(petaShock, sizePetaShock);
      }
    }
  } else {
    delayMicroseconds(270);
    digitalWrite(COLLAR_IN_PIN, random(2)); // Feed garbage when not receiving input
  }
}