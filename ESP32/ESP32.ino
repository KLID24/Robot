#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP32.h>

#include "secrets.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define STORE_PATH "/user/name"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseESP32 firebase;

String Client_Name = "";

void setup_lcd()
{
	lcd.init();
	lcd.backlight();
	lcd.setCursor(0, 0);
}

void lcd_print_new(String msg)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(msg);
}

void setup_wifi() 
{
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	while (WiFi.status() != WL_CONNECTED) 
    {
		Serial.println("Connecting wifi");
		Serial.print(".");
		delay(500);
	}
	
	Serial.println("WiFi connected!");
} 

void setup_firebase() 
{
	config.api_key = API_KEY;
	config.database_url = DATABASE_URL;

	firebase.begin(&config, &auth);
	firebase.reconnectWiFi(true);

	if (firebase.signUp(&config, &auth, "", "")) 
    {
    	Serial.println("sing-up: OK");
    	lcd_print_new("Sign-up is done!");
 	} 
	else 
    {
		Serial.println("sign-up: FAIL");
	} 

	delay(2000);
}

void set_name() 
{
	unsigned int len = Client_Name.length();
	unsigned int padding = (16 - len) / 2;

	Serial.println("Name set: " + Client_Name);

	if (padding < 0) {
   	    padding = 0;
  	}
  
    lcd.clear();
	lcd.setCursor(2, 0);
	lcd.print("Pedido para:");
	lcd.setCursor(padding, 1);
	lcd.print(Client_Name);
}

void setup() 
{
	Serial.begin(115200);
	setup_lcd();

	lcd_print_new("Connecting WiFi..");
	setup_wifi();

	lcd_print_new("WiFi Connected!");
	setup_firebase();
}


void loop() {
    static unsigned long last_check = 0;
    const unsigned long check_interval = 5000;

    if (millis() - last_check >= check_interval) {
        last_check = millis();

        if (Firebase.getString(fbdo, STORE_PATH)) {
            String new_name = fbdo.stringData();

            if (new_name != Client_Name) {
                Client_Name = new_name;
                Serial.println("Name changed: " + Client_Name);
                set_name();
            } 
            else {
                Serial.println("Name didn't change.");
            }
        } 
        else {
            Serial.println("Could not read from firebase: " + fbdo.errorReason());
        }
    }
}