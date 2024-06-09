// Adafruit M0 Lora //
#define button_pin A0

unsigned int OffDelayMs = 4000;

// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>

// First 3 here are boards w/radio BUILT-IN. Boards using FeatherWing follow.
#define RFM95_CS 8
#define RFM95_INT 3
#define RFM95_RST 4



// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {

   pinMode(button_pin, INPUT);

 // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  //while (!Serial) delay(1);
  delay(100);

  Serial.println("Feather LoRa RX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1)
      ;
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1)
      ;
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

uint8_t expected_buf[] = { 98, 117, 116, 116, 0 };

unsigned long stamp;

bool stampON = false;


unsigned int delayButtonPresses = 35;

void loop() {

   if(millis() - stamp >= OffDelayMs && stampON == true){

    stampON = false;

    turnOff();

  }

  if (rf95.available()) {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
     //digitalWrite(LED_BUILTIN, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);

      //       Serial.print("Got: ");
      //       for(int i = 0; i<5; i++){
      // Serial.print(buf[i]);
      // Serial.print(",");
      //       }
      //       Serial.println();

      bool they_match = true;             // assume they match
      for (int i = 0; i < 5; i++) {       // for each note/learn element
        if (buf[i] != expected_buf[i]) {  // check if they don't match
          they_match = false;             // they don't mach, so set flag
          break;                          // stop looping
        }
      }

      if (they_match == true && stampON == false) {
        Serial.println("RECEIVED FROM EMITTER");

        stampON = true;

        turnOnSolid();
      }




      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "Received";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      //digitalWrite(LED_BUILTIN, LOW);
     } //else {
    //   Serial.println("Receive failed");
    // }
  }
}

void turnOnSolid() {
  for (int i = 0; i < 4; i++) {
    pinMode(button_pin, OUTPUT);
    digitalWrite(button_pin, LOW);
    delay(delayButtonPresses);
    pinMode(button_pin, INPUT);
    delay(delayButtonPresses);
  }

  stamp = millis();
}

void turnOff(){
  pinMode(button_pin, OUTPUT);
    digitalWrite(button_pin, LOW);
    delay(100);
    pinMode(button_pin, INPUT);
    delay(100);
}
