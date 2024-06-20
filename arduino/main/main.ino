#include <WiFi.h>
#include <WiFiUdp.h>

const char* SSID = "SSID";
const char* pass = "PASS";
unsigned int port = 4120; 
char incomingPacket[4]; //Packet buffer size.
WiFiUDP Udp;

int beepDat[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}; //Temporary! Ideally, these can be gen on the fly.
int sizeBeepDat = sizeof(beepDat) / sizeof(beepDat[0]);

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

#define COLLAR_IN_PIN 23
#define LED_BUILTIN 2

void setup() {
  Serial.begin(9600);
  pinMode(COLLAR_IN_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
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
    Serial.printf("Running at IP %s, UDP port %d - listening.\n", WiFi.localIP().toString().c_str(), port);
}

void triggerOn(int* dat, int size, int wait = 270) {
  Serial.println("Injecting data!");
  // int32_t btime, etime;
  for (int i = 0; i < size; i++) {
    //Serial.print(dat[i]);
    // btime = asm_ccount();
    digitalWrite(COLLAR_IN_PIN, dat[i]);

    // if (dat[i] == 1) {
    //   GPOS = (1 << COLLAR_IN_PIN);      
    // } else {
    //   GPOC = (1 << COLLAR_IN_PIN); 
    // }
    // etime = asm_ccount();
    // Serial.println((uint32_t)(etime - btime));    
    // delayMicroseconds(wait-((uint32_t)(etime - btime)/80000));
    delayMicroseconds(wait);
  }
  digitalWrite(COLLAR_IN_PIN, 0); // Set data pin to low after sequence, collar expects this.
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

void loop() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        int len = Udp.read(incomingPacket, 4);
        if (len > 0)
        {
          incomingPacket[len] = '\0';
        }
        Serial.println("Got packet. Packet received:");
        Serial.println(incomingPacket);
        for (int i = 0; i <= 3; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(20);
            digitalWrite(LED_BUILTIN, LOW);
            delay(20);
        }
        if (incomingPacket[0] == '1') {
            Serial.println("Beep received. Injecting beep...");
            triggerOn(beepDat, sizeBeepDat);
        } else if (incomingPacket[0] == '0') {
          // Serial.println("Shock received, injecting shock at specified power...");
          if (incomingPacket[1] == '1') {
            Serial.println("Injecting shock at power 1!");
            // Serial.println(__measure(triggerOn(shock, sizeShock));
//             __measure(triggerOn(shock, sizeShock);
            // int32_t btime, etime;
            // btime = asm_ccount();
            triggerOn(shock, sizeShock);
            // etime = asm_ccount();
            // Serial.println((uint32_t)(etime - btime));  
          } else if (incomingPacket[1] == '2') {
            Serial.println("Injecting shock at power 2!");
            triggerOn(kiloShock, sizeKiloShock);
          } else if (incomingPacket[1] == '3') {
            Serial.println("Injecting shock at power 3!");
            triggerOn(megaShock, sizeMegaShock);
          } else if (incomingPacket[1] == '4') {
            Serial.println("Injecting shock at power 4!");
            triggerOn(gigaShock, sizeGigaShock);
          } else if (incomingPacket[1] == '5') {
            Serial.println("Injecting shock at power 5!");
            triggerOn(teraShock, sizeTeraShock);
          } else if (incomingPacket[1] == '6') {
            Serial.println("Injecting shock at power 6!");
            triggerOn(petaShock, sizePetaShock);
          }
        }
    } else {
      delayMicroseconds(270);
      digitalWrite(COLLAR_IN_PIN, random(2)); // Feed collar garbage while not sending command   
    }
}