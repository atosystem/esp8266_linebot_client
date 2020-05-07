#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "MakerSpace_2.4G";
const char* pass = "ntueesaad";

const byte BTN_PIN = 0;     // 按鈕/開關接腳
const byte LED_PIN = 2;     // LED接腳

unsigned long previousMillis = 0;
const long interval = 60000;     // 60秒

HTTPClient http;

void line() {
    http.begin("testing10293.herokuapp.com", 80, "/send");
    int httpCode = http.GET();
    if(httpCode > 0) {
        // 在序列埠監控視窗顯示送出的數據
        Serial.printf("HTTP code: %d\n", httpCode);
        
        if(httpCode == 200) {
          String payload = http.getString();
          // 顯示遠端伺服器的回應
          Serial.println(payload);
        }
    } else {
        Serial.println("HTTP connection ERROR!");
    }
}

void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   pinMode(BTN_PIN, INPUT_PULLUP); 
  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi ready...");
  line();
}

void loop() {
    // unsigned long currentMillis = millis();
    // boolean btnState= digitalRead(BTN_PIN);
    
    // if (btnState == LOW) {
    //   // 點亮LED
    //   digitalWrite(LED_PIN, LOW);
    //   if ((previousMillis == 0) || (currentMillis - previousMillis) >= interval) {
    //     previousMillis = currentMillis;
    //     line();
    //   }
    // } else {
    //   // 關閉LED
    //   digitalWrite(LED_PIN, HIGH);
    // }
    delay(2000);
 }