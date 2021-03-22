#include <SPI.h>
#include <MFRC522.h>
#include <LedControl.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define RFID_SS_PIN 10
#define RFID_RST_PIN 9

#define DISPLAY_CS_PIN 3
#define DISPLAY_DATA_PIN 4
#define DISPLAY_CLK_PIN 5

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define NUMBER_OF_DISPLAYS 1

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DISPLAY_DATA_PIN, DISPLAY_CLK_PIN, DISPLAY_CS_PIN, NUMBER_OF_DISPLAYS);

void setup() {
  setUpRfidScanner();
  setUpDisplay();
}

void setUpRfidScanner() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void setUpDisplay() {
  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();
}

void loop() {
  animateText();

  if (noCardIsNearBy()) {
    return;
  }

  if (cardIdIsValid(getCardId())) {
    myDisplay.displayText("Access Granted", PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  } else {
    myDisplay.displayText("Access Denied", PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}

void animateText() {
  if (myDisplay.displayAnimate()) {
    myDisplay.displayClear();
  }
}

boolean noCardIsNearBy() {
  return !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial();
}

boolean cardIdIsValid(String cardId) {
  return cardId == "67 FA 80 C9";
}

String getCardId() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}
