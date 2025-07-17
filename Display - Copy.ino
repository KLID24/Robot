#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP32.h>
#include "secrets.h"  // WIFI_SSID, WIFI_PASSWORD, API_KEY, DATABASE_URL, USER_PATH

LiquidCrystal_I2C lcd(0x27, 16, 2);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseESP32 firebase;  // используем новый стиль

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");

  // Настройка Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = "";
  auth.user.password = "";
  firebase.begin(&config, &auth);
  firebase.reconnectWiFi(true);

  delay(2000);

  // Новый способ чтения данных из Firebase
if (firebase.getString(fbdo, USER_PATH "/name")) {
  String name = fbdo.stringData();
  Serial.println("Name from Firebase: " + name);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Client:");
  lcd.setCursor(0, 1);
  lcd.print(name);
} else {
  Serial.println("Failed to read from Firebase: " + fbdo.errorReason());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Firebase Error");
  lcd.setCursor(0, 1);
  lcd.print("Check Serial");
}
}

void loop() {
  // Здесь может быть обновление или автообновление
}