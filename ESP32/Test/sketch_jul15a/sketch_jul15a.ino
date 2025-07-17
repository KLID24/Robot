#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Personal-661"
#define WIFI_PASSWORD "0018781896"
#define API_KEY "AIzaSyDm52J6WTHvZyqRU4czpy5lYyuZ0TVykLQ"
#define DATABASE_URL "https://srpapa-database-default-rtdb.firebaseio.com/"
#define USER_PATH "/user"

LiquidCrystal_I2C lcd(0x27, 16, 2);

// объекты для Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseESP32 firebase;
String name = "";

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

  // Инициализация Firebase
  if(firebase.signUp(&config, &auth, "", "")){
    Serial.println("singUp  OK");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("signUp is done!");
  }

  delay(4000);
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  delay(1000);

  UpdateName();
}

void UpdateName () {
  if (Firebase.getString(fbdo, USER_PATH "/name")) {
    name = fbdo.stringData();
    Serial.println("Name from Firebase: " + name);
    lcd.clear();
    int len = name.length();
    int padding = (16 - len) / 2;
    padding = max(0, padding);

    lcd.setCursor(padding, 0);
    lcd.print("Client:");
    lcd.setCursor(padding, 1);
    lcd.print(name);
  } else {
    Serial.println("Failed to get data: " + fbdo.errorReason());
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error reading DB");

  }
}

void check

void loop() {
  delay(3000);
  UpdateName();
}
