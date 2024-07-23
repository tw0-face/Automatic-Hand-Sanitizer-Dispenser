// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLxio87V9A"
#define BLYNK_DEVICE_NAME "ESP32"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7
//#define USE_ESP32C3_DEV_MODULE
//#define USE_ESP32S2_DEV_KIT

#include "BlynkEdgent.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* url = "https://services1.arcgis.com/0MSEUqKaxRlEPj5g/arcgis/rest/services/ncov_cases/FeatureServer/1/query?f=json&where=(Country_Region=%27EGYPT%27)&returnGeometry=false&outFields=Country_Region,Confirmed,Deaths";


const int trigPin = 13 ;
const int echoPin = 12;
const int pump = 14;
long duration;
int distance;

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  pinMode(pump, OUTPUT);

  digitalWrite(pump, HIGH); 
}

void ultra(){

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.0340 / 2;

  Serial.println("Distance");

  Serial.println(distance);

  if (distance <= 15){

    Serial.print("Opening Pump");

    digitalWrite(pump, LOW);

    delay(2000);

    digitalWrite(pump, HIGH);

    }

}

void lcd_data(){
       HTTPClient https;
  String data;

  https.begin(url);

  int httpCode = https.GET();

    String payload = https.getString();

    char charBuf[500];

    payload.toCharArray(charBuf, 500);   

    //Serial.println(payload);

    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 3 * JSON_OBJECT_SIZE(6) + 2 * JSON_OBJECT_SIZE(7) + 690;

    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload);

    JsonArray fields = doc["fields"];

    JsonObject features_0_attributes = doc["features"][0]["attributes"];


    int features_0_attributes_Confirmed = features_0_attributes["Confirmed"];

    int features_0_attributes_Deaths = features_0_attributes["Deaths"];



String z = "Confirmed :" + String(features_0_attributes_Confirmed);
String y = "Deaths :" + String(features_0_attributes_Deaths);
 https.end();
      Blynk.virtualWrite(V0,z ); 

    Blynk.virtualWrite(V1,y );  

}

void loop() {
        ultra();
  BlynkEdgent.run();
      lcd_data();
 
}
