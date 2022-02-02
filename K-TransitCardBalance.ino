
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

#define OLED_RESET -1   // S/W Reset. (OLED 를 소프트웨어 적으로 초기화하는 코드입니다. 사용되지는 않습니다.)

// To use the PN532 module in SPI mode, Firstly changing the switch,
// then a total of 6 wires must be connected, including VCC and GND pins.
// Afterward, define the connection pins specified based on the NodeMCU ESP8266-12E.
// PN532 모듈을 SPI 모드로 사용하기 위해서는 스위치를 변경한 후
// VCC, GND 핀을 포함하여 총 6개의 선 연결이 필요합니다.
// NodeMCU ESP8266-12E 기준으로 지정한 연결 핀들을 입력해줍니다.
#define PN532_SCK  (D5) //
#define PN532_MOSI (D7) //
#define PN532_SS   (D4) //
#define PN532_MISO (D6) //

// To use the 'I2C mode', the two pins below must be connected.
// I2C 모드를 이용하기 위해서는 아래 2 개의 핀 연결이 필요합니다.
#define PN532_IRQ   (2)  // Not used.
#define PN532_RESET (0)  // Not connected by default on the NFC Shield.

// BEEP Pin 을 지정해줍니다.
#define  BEEP_PIN (D8)
 
// LED Pin 을 지정해줍니다.
#define LED_PIN  (LED_BUILTIN)

//--------------------------------------------------------------
// Global variables
//--------------------------------------------------------------

SSD1306 display(0x3c, D2, D1); // Object declaration for OLED screen display. (OLED 화면 표시용 객체입니다.)
Adafruit_PN532 nfc(PN532_SS);  // Object declaration for NFC capabilities. (NFC 통신용 객체입니다.)

//--------------------------------------------------------------
// Setup function
//--------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(50);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Disables a dedicated LED light on the ESP8266 chipset. (내장된 식별 LED 를 끕니다.)
  Serial.println("Start NFC");

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board"); // Reboot if the PN532 is not recognized. (PN532 를 인식하지 못한 경우 재부팅합니다.)
    while (1);
  }

  // If the PN532 is connected and recognized, display information related to the chipset on the console log.
  // PN532 가 연결되어 인식되면 콘솔 로그에 칩셋 관련 정보를 표시합니다.
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);


  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Connect Oled as an I2C method, and its address is 0x3c.
  // Initializing the OLED.
  // OLED 를 I2C 방식으로 연결하시면, 그 주소는 0x3c 로 지정되어 있습니다.
  // OLED 를 초기화 합니다.
  display.init();
  display.flipScreenVertically();

  // RFID
  // Configure board to read RFID tags.
  // RFID 태그를 읽을 수 있도록 보드를 구성합니다.
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

char  card_id[16 + 1] = ""; // Card No. (16-bytes) 카드 일련번호
char  date_issued[8 + 1] = ""; // Date issued. (8-bytes) 카드 최초 발행일
char  card_issuer = 0x00;  // Card issuer  카드 발행사
char  card_type = 0x00;    // Card type    카드 종류

void loop() {
  uint8_t success;
  uint8_t responseLength = 64;
  success = nfc.inListPassiveTarget();
  if (success) {
    Serial.println("Found something!");
    // This code is for reading the serial number or issued date of the T-money card.
    // 티머니 카드의 일련 번호 또는 발급 날짜를 읽기 위한 코드입니다.
    uint8_t cardInfo[responseLength];
    uint8_t cardNumSize = 0;
    uint8_t selectApdu[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x42, 0x00 };
    success = nfc.inDataExchange(selectApdu, sizeof(selectApdu), cardInfo, &responseLength);
    if (success) {
      Serial.print("responseLength: "); Serial.println(responseLength);
      nfc.PrintHexChar(cardInfo, responseLength);
      if (responseLength >= 24) {

        CharToHex( cardInfo + 13, card_id, 3);   // Card No. (카드 일련번호)
        // CharToHex( cardInfo + 8, card_id, 8);
        CharToHex( cardInfo + 21, date_issued, 3);  // Date issued. (카드 최초 발행일)
        // CharToHex( cardInfo + 21, date_issued, 4);
        card_issuer = cardInfo[7];   // Card issuer (카드 발행사)
        card_type = cardInfo[29];  // Card type (카드 종류)
        Serial.print("card number : "); nfc.PrintHexChar(cardInfo + 8, 8);
        Serial.print("date issued : "); nfc.PrintHexChar(cardInfo + 21, 4);
        Serial.print("card issuer : ");  Serial.println(issuer_corps( (int)card_issuer) );
        Serial.print("card type : "); Serial.println(user_type((int)card_type) );
      }
    } else {
      Serial.print("Not recognized");
      display.clear();
      display.display();
      display.drawString(0, 0, ": (  Unrecognized Card!");
      display.drawString(0, 17, "Please tag your prepaid");
      display.drawString(0, 29, "T-Money card to this");
      display.drawString(0, 41, "device."); 
      display.display();
      beep_error();
      delay(1000);
      return;
    }

  // Used for balance inquiry purposes.
  // When the inquiry is complete, the remaining balance will be display on the OLED screen.
  // 잔액 조회 용도로 사용되는 코드입니다.
  // 조회가 완료되면 남은 잔액을 OLED 에 표시합니다.
  uint8_t balance[responseLength];
  uint8_t balanceApdu[] = { 0x90, 0x4C, 0x00, 0x00, 0x04 } ;
  success = nfc.inDataExchange(balanceApdu, sizeof(balanceApdu), balance, &responseLength);
  if (success) {
    Serial.print("responseLength: "); Serial.println(responseLength);
    nfc.PrintHexChar(balance, responseLength);
    if (responseLength >= 4) {
      char fpsbuf[32] = ""; // It is used to convert numbers into strings and display them on the screen. (숫자를 문자열로 변환하여 화면에 출력하는 용도로 사용됩니다.)
      uint32_t credit = balance[0] * 256 * 256 * 256 +  balance[1] * 256 * 256 +  balance[2] * 256 + balance[3];
      display.clear();
      memset(card_id + 0, 'x', 2);  // For extra security, only the last 4 digits of the card's serial number are displayed. (보안성 강화를 위해 카드의 일련번호 맨 끝 4 자리만 표시합니다.)
      display.drawString(0, 0, card_id);
      display.drawString(39, 0, date_issued);
      display.drawString(78, 0, user_type((int)card_type));
      dtostrf((float)credit, 10, 0, fpsbuf);
      display.setFont(ArialMT_Plain_24);
      display.drawString(0, 11, fpsbuf);
      display.setFont(ArialMT_Plain_10);
      if (credit < 1700) {
        display.drawString(0, 32, "The balance on your");
        display.drawString(0, 42, "card is critically low.");
        display.drawString(0, 52, "Please recharge it NOW.");
      } else if (credit < 4000) {
        display.drawString(0, 32, "You have less than");
        display.drawString(0, 42, "4,000 KRW on your card.");
        display.drawString(0, 52, "Please recharge it ASAP.");
      } else {
        display.drawString(0, 32, "Have a great day and");
        display.drawString(0, 42, "stay safe while travelling.");
        display.drawString(0, 52, "* ^ - ^ *");
      }
      display.display();
      beep();
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
  nfc.SAMConfig();
  display.display();
}
delay(100);
}

void CharToHex(uint8_t *ch, char* szHex, int len)
{
  unsigned char saucHex[] = "0123456789ABCDEF";
  for (int  i = 0; i < len; i++) {
    szHex[i * 2 + 0] = saucHex[ch[i] >> 4];
    szHex[i * 2 + 1] = saucHex[ch[i] & 0xF];
  }
}

char* issuer_corps( int idx ) {
  switch (idx) {
    case 1:  return "KFTC"; // Korea Financial Telecomunications & Clearings Institute
    case 2:  return "A-Cash";
    case 3:  return "Mybi";
    case 5:  return "V-Cash";
    case 6:  return "Mondex Korea";
    case 7:  return "Korea Expressway Corp.";
    case 8:  return "T-Money";  //
    case 9:  return "Korail";   // The Korea Railroad Corporation
    case 11:  return "EB Card"; // Cash-Bee
    case 12:  return "SMBA";    // Seoul Metropolitan Bus Association
  }
  return "Unspecified";
}

char* user_type( int idx ) {
  switch (idx) {
    case 1:  return "Regular";
    case 2:  return "Children";
    case 3:  return "Seniors";
    case 4:  return "Teenagers";
    case 5:  return "Disabled";
  }
  return "Card Type : Unspecified";
}

void beep() {
  digitalWrite(LED_PIN, LOW);
  tone(BEEP_PIN, 4500, 60);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
}

void beep_error() {
  digitalWrite(LED_PIN, LOW);
  tone(BEEP_PIN, 2500, 700);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
}
