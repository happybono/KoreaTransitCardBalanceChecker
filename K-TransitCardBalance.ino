//
//    FILE: K-TransitCardBalance.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

#include <Arduino.h>
#include "Adafruit_PN532.h"
#include "SSD1306.h"

#define OLED_RESET -1 

#define PN532_SCK  (D5) //
#define PN532_MOSI (D7) //
#define PN532_SS   (D0) //
#define PN532_MISO (D6) //

#define PN532_IRQ   (2)  // Not Use
#define PN532_RESET (0)  // Not connected by default on the NFC Shield

//--------------------------------------------------------------
// Global variables
//--------------------------------------------------------------

SSD1306 display(0x3c, D2, D1);
Adafruit_PN532 nfc(PN532_SS);    

//--------------------------------------------------------------
// Setup function
//--------------------------------------------------------------

void setup(){
  Serial.begin(115200);
  delay(50);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.println("Start NFC");

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);

 
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  display.init();
  display.flipScreenVertically();
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A Card ...");
  display.display(); 
  delay(2000);

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "T-Money Balance Checker");
  display.drawString(1, 17, "To check the balance,");
  display.drawString(1, 29, "please tag your prepaid");
  display.drawString(1, 41, "T-Money card to this device.");
  display.display();
}

//--------------------------------------------------------------
// loop function
//--------------------------------------------------------------

void loop(){
  uint8_t success;
  uint8_t responseLength = 64;
  success = nfc.inListPassiveTarget();
  if (success) {
    Serial.println("Found something!");
    uint8_t cardInfo[responseLength];
    uint8_t cardNumSize = 0;
    uint8_t selectApdu[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x42, 0x00 };
    success = nfc.inDataExchange(selectApdu, sizeof(selectApdu), cardInfo, &responseLength);
    if (success) {
      Serial.print("responseLength: "); Serial.println(responseLength);
      nfc.PrintHexChar(cardInfo, responseLength);
      if (responseLength >= 24) {
        Serial.print("card number : "); nfc.PrintHexChar(cardInfo + 8, 8);
        Serial.print("card date : ");   nfc.PrintHexChar(cardInfo + 21, 4);
      }
    }

    uint8_t balance[responseLength];
    uint8_t balanceApdu[] = { 0x90, 0x4C, 0x00, 0x00, 0x04 } ;
    success = nfc.inDataExchange(balanceApdu, sizeof(balanceApdu), balance, &responseLength);
    if (success) {
      Serial.print("responseLength: "); Serial.println(responseLength);
      nfc.PrintHexChar(balance, responseLength);
      if (responseLength >= 4) {
        char fpsbuf[32] = "";
        uint32_t credit = balance[0] * 256 * 256 * 256 +  balance[1] * 256 * 256 +  balance[2] * 256 + balance[3];
        display.clear();
        display.drawString(0, 0, "Balance left on this card : ");
        dtostrf((float)credit, 10, 0, fpsbuf);
        display.setFont(ArialMT_Plain_24);
        display.drawString(0, 11, fpsbuf);
        display.setFont(ArialMT_Plain_10);
        if (credit < 1500) {
          display.drawString(0, 32, "The balance on your");
          display.drawString(0, 42, "card is critically low.");
          display.drawString(0, 52, "please recharge it now.");      
          } else if (credit < 3500) {
          display.drawString(0, 32, "You have less than");
          display.drawString(0, 42, "3000 won on your card.");
          display.drawString(0, 52, "please recharge it ASAP.");
          } else {
          display.drawString(0, 34, "Have a great day and");
          display.drawString(0, 46, "safe travel ! * ' - ' *");
          }
        display.display();
        delay(5000);
        Serial.print("Balance left on this card : "); Serial.println(credit);
      }
    }
  }
  else {
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "T-Money Balance Checker");
  display.drawString(1, 17, "To check the balance,");
  display.drawString(1, 29, "please tag your prepaid");
  display.drawString(1, 41, "T-Money card to this device.");
  display.display();
  nfc.SAMConfig();
  }
  delay(100);
}
