#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>      
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "MojaTV_Full_206355"
#define WIFI_PASSWORD "osman23016osman23016"
#define API_KEY "AIzaSyA8OW9Svs7nLclPIfFC79k7EUM3tZeoli4"
#define DATABASE_URL "https://temperature-humidity-iot-eb42a-default-rtdb.europe-west1.firebasedatabase.app/"

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase objekti
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// Definicija pinova
#define DHTPIN D4
#define DHTTYPE DHT11
#define GREEN_LED D1
#define RED_LED D2
#define BUZZER D3

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  // Firebase inicijalizacija
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase OK");
    signupOK = true;
  } else {
    Serial.printf("Firebase ERROR: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
    
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Kontrola LED dioda i buzzera
  bool greenLedState, redLedState, buzzerState;

  if (temperature < 28) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    greenLedState = true;
    redLedState = false;
    buzzerState = false;
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);  
    greenLedState = false;
    redLedState = true;
    buzzerState = true;
  }

  // Slanje podataka u Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setFloat(&fbdo, "DHT_11/Temperature", temperature);
    Firebase.RTDB.setFloat(&fbdo, "DHT_11/Humidity", humidity);
    Firebase.RTDB.setString(&fbdo, "DHT_11/Green_LED_State", greenLedState ? "ON" : "OFF");
    Firebase.RTDB.setString(&fbdo, "DHT_11/Red_LED_State", redLedState ? "ON" : "OFF");
    Firebase.RTDB.setString(&fbdo, "DHT_11/Buzzer_State", buzzerState ? "ON" : "OFF");

    Serial.print("Temperature: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
    Serial.print("Green LED: "); Serial.println(greenLedState ? "ON" : "OFF");
    Serial.print("Red LED: "); Serial.println(redLedState ? "ON" : "OFF");
    Serial.print("Buzzer: "); Serial.println(buzzerState ? "ON" : "OFF");
  }
}
