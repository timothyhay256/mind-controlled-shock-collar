/*
Originally developed by Smouldery and Mikey who very kindly allowed us the use of their code. 

Link to Smoulderys original code: https://github.com/smouldery/shock-collar-control/blob/master/Arduino%20Modules/transmitter_vars.ino

adapted by Deviant Designs

Link to Deviant Design work: http://deviant-designs.co.uk/2019/03/29/arduino-controlled-shock-collar/

some timing changes to match the new RF protocol by DpMartee

Adapted to accept commands over UDP by timothyhay
Packet buffer accepts commands in the following format:
0 0 000  
| |  |
| | collar_power
| |
| mode
|
channel

*/


#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>

const char* SSID = "STN";
const char* pass = "88bb6b7054";
unsigned int port = 4120; 
char incomingPacket[5]; // Packet buffer.
// The following have a increased length to allow for adding an null terminator to the end.
char activePower[4]; // Power level buffer
char activeChannel[2]; // Channel buffer
char activeMode[2]; // Mode buffer
WiFiUDP Udp;

//=================================================== START OF COLLAR SETUP CODE ======================================================================

//const int shock_min = 0; // Minimum of power a command will be executed at
const int shock_delay = 10; // Maximum rate at which the shock function can be used at
//const int cmd_max = 1000; // Maximum of milliseconds which a command can be executed at

// Constant variables
const int pin_led = LED_BUILTIN; // Pin for indication LED
const int pin_rtx =  D2; // Pin to transmit over
const String key = "00101100101001010"; // Key of the transmitter, dont touch if you dont know how it works

// Variables which do change
int collar_chan = 0; // Can be channel 0 or 1. Can be set over UDP.
int collar_duration = 500; // Duration of the command in milliseconds. Seems to be useless.
int collar_power = 10; // Strength of the command, can be 0-100, but will be limited by shock_min and shock_max. Can be set over UDP
int collar_mode = 3; // See below

// Define values for easier recognition
#define COLLAR_LED 1
#define COLLAR_BEEP 2
#define COLLAR_VIB 3
#define COLLAR_ZAP 4

// Strings used for building up the command sequence
String sequence, power, channelnorm, channelinv, modenorm, modeinv;

// Store the last time anything was transmitted to the collar
unsigned long transmit_last = 0;
unsigned long shock_last = 0;
void transmit_command(int c, int m, int p = 0)
{
  transmit_last = millis();
  switch (c) // Check the channel
  {
    case 1: // Channel 1
      channelnorm = "111";
      channelinv = "000";
      break;
    default: // Channel 0
      channelnorm = "000";
      channelinv = "111";
      break;
  }

  switch (m) // Check the mode
  {
    case 1: // Light
      modenorm = "1000";
      modeinv = "1110";
      break;
    case 2: // Beep
      modenorm = "0100";
      modeinv = "1101";
      break;
    case 4: // Shock
      modenorm = "0001";
      modeinv = "0111";
      shock_last = millis();
      break;
    default: // Vibrate
      modenorm = "0010";
      modeinv = "1011";
//      p = 10; // Set strengh to 10 for the command to be executed properly
      break;
  }

  // Convert power to binary
  int zeros = String(p, BIN).length();

  String power;
  for (int i = 0; i < 7 - zeros; i++)
  {
    power = power + "0";
  }
  power = power + String(p, BIN);

  String sequence = "1" + channelnorm + modenorm + key + power + modeinv + channelinv + "00";

  digitalWrite(pin_led, LOW);
//  d = constrain(d, 50, cmd_max); // Clamp duration of the command
  unsigned long cmd_start = millis();
//  while (millis() - cmd_start < d)
//  {
    // start bit
    digitalWrite(pin_rtx, HIGH);
    delayMicroseconds(400); // chnged to new protocol
    digitalWrite(pin_rtx, LOW);
    delayMicroseconds(750);// wait 750 uS

    for (int n = 0; n < 41 ; n++)
    {
      if (sequence.charAt(n) == '1') // Transmit a one
      {
        digitalWrite(pin_rtx, HIGH);
        delayMicroseconds(200); // chnged to new protocol
        digitalWrite(pin_rtx, LOW);
        delayMicroseconds(1500); // chnged to new protocol
      }
      else // Transmit a zero
      {
        digitalWrite(pin_rtx, HIGH);
        delayMicroseconds(200); // chnged to new protocol
        digitalWrite(pin_rtx, LOW);
        delayMicroseconds(750); // chnged to new protocol
      }
    }
    delayMicroseconds(9000); // chnged to new protocol
//  }
  digitalWrite(pin_led, HIGH);
}

void collar_keepalive()
{
  if (millis() - transmit_last >= 120000) // Send command to the collar at least every 2 minutes to make it stay on
  {
    Serial.println("Keep-alive:\tCollar");
    transmit_command(collar_chan, COLLAR_LED, 50);
  }
}

//=================================================== END OF COLLAR SETUP CODE ======================================================================

void setup()
{
//=================================================== START OF COLLAR SETUP CODE ======================================================================
  pinMode(pin_rtx, OUTPUT); // Set transmitter pin as output
  pinMode(pin_led, OUTPUT); // Set LED pin as output
  Serial.begin(115200);
  //=================================================== END OF COLLAR SETUP CODE ======================================================================
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

void loop()
{
  collar_keepalive();

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 9);
    if (len > 0) {
        incomingPacket[len] = '\0';
    }
    Serial.println("Got packet. Packet received:");
    Serial.println(incomingPacket);
        for (int i = 0; i <= 3; i++) { // Visually indicate received packet
        digitalWrite(LED_BUILTIN, HIGH);
        delay(20);
        digitalWrite(LED_BUILTIN, LOW);
        delay(20);
    }

    activeChannel[0] = incomingPacket[0];
    activeChannel[1] = '\0';

    activeMode[0] = incomingPacket[1];
    activeMode[1] = '\0';
    activePower[0] = incomingPacket[2];
    activePower[1] = incomingPacket[3];
    activePower[2] = incomingPacket[4];
    activePower[3] = '\0';

    Serial.print("collar_chan: ");
    Serial.println(atoi(activeChannel));

    Serial.print("collar_mode: ");
    Serial.println(atoi(activeMode));

    Serial.print("activePower: ");
    Serial.println(atoi(activePower));

    transmit_command(atoi(activeChannel), atoi(activeMode), atoi(activePower));
    transmit_command(atoi(activeChannel), atoi(activeMode), atoi(activePower));
//   if (digitalRead(buttonPin) == LOW) {
    // transmit_command(collar_chan, COLLAR_VIB, 100);
//   }
  }
}