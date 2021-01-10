# TMoneyCardBalanceChecker

<div align="center">
<img alt="GitHub Last Commit" src="https://img.shields.io/github/last-commit/happybono/TMoneyCardBalanceChecker"> 
<img alt="GitHub Repo Size" src="https://img.shields.io/github/repo-size/happybono/TMoneyCardBalanceChecker">
<img alt="GitHub Repo Languages" src="https://img.shields.io/github/languages/count/happybono/TMoneyCardBalanceChecker">
<img alt="GitHub Top Languages" src="https://img.shields.io/github/languages/top/HappyBono/TMoneyCardBalanceChecker">
</div>

## What's New
### January 03, 2021
>[Initial release.](https://github.com/happybono/TMoneyCardBalanceChecker/commit/fec9bfa518c8a4e4afe6bc7d3de6c8143084da48)

### January 05, 2021
>[Added README.md](https://github.com/happybono/TMoneyCardBalanceChecker/commit/ea930c71896c7cd582c2e9f4dfeed69acf5db815) </br>
>[Comments have added to each function in the source code.](https://github.com/happybono/TMoneyCardBalanceChecker/commit/74afa73fddc93052ed60340b95d96530603bea3d#diff-87f942957af3e245df937cc59e8e84242eb4c3a3b22ce052b06e2693e99a277e)

## Specifications
![Tmoney.png](Tmoney.png)

## Schematics
![T-Money-Checker-Schematics.png](GIT_TMoneyBalCkr_SCHEM.png)

## Apparatus (Equipment)
### Platform
* NodeMCU ESP8266-12E

### PN532 NFC / RFID Controller
* Soldering required.
* Connection Type : SPI
* D0 : SS of PN532
* D5 : TX of PN532
* D6 : MISO of PN532
* D7 : MOSI of PN532
* GND : GND of PN532
* VIN : VCC of PN532

### 0.96" SSD1306 I2C OLED Display 
* Soldering required
* D2 : Data, D1 : Clock

## Features
* Tapping the T-money card to the NFC module displays the remaining balance on the OLED screen.
> The T-money card is a rechargeable smart card that can be used to pay public transit fares in the Seoul-Gyeonggi areas as well in other cities around Korea (bus, underground/metro, taxi) as well as purchases in designated areas. The T-money card can be used to pay subway and bus and taxi travel.

<div align="center">
<img src="https://github.com/happybono/TMoneyCardBalanceChecker/blob/main/CI_Eng_H_Color_G.jpg" width=17% height=17%>
</div>

## Libraries Used.
### Adafruit-PN532
This is a library for the Adafruit-PN532 NFC / RFID breakout boards This library works with the Adafruit NFC breakout.
https://github.com/adafruit/Adafruit-PN532

### SSD1306 (0.96") OLED Display
This is a driver for SSD1306 128x64, 128x32, 64x48 and 64x32 OLED displays running on the Arduino/ESP8266 & ESP32 and mbed-os platforms. Can be used with either the I2C or SPI version of the display.
https://www.github.com/ThingPulse/esp8266-oled-ssd1306

## Copyright / End User License
### Copyright
Copyright ⓒ HappyBono 2021. All rights Reserved.

### MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Contact Information
[Jaewoong Mun](mailto:happybono@outlook.com)

## References
* Hyunwoo Nam, Byunggeun Jang, Youngwook Park, 2012, A Design of Card Processing Method based on Script File for AFC Terminal, KoreaScience, Journal of The Korea Society of Computer and Information (JKSCI), viewed 5 January 2021, https://www.koreascience.or.kr/article/CFKO201202135240043.pdf.
