#include <SPI.h>
#include <MFRC522.h>
#include <LedControl.h>

// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1
#define CS_PIN 3
#define DATA_PIN 4
#define CLK_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

boolean authenticated = false;

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  setUpRfidScanner();
  setUpDisplay();
}

void loop() {
  if (authenticated) {
    if (myDisplay.displayAnimate()) {
      myDisplay.displayReset();
      authenticated = false;
    }
  }

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String cardId = getCardId();
  if (isAuthorized(cardId)) {
    myDisplay.displayText("Access Granted", PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    authenticated = true;
  } else {
    myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print("X");
    delay(1000);
    myDisplay.displayClear();
  }
}

boolean isAuthorized(String cardId) {
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
