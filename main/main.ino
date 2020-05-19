#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <ArduinoJson.h>

// for json msg parsing
StaticJsonDocument<477> doc;
DeserializationError error;

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// uint8_t on_state[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02, 0x20, 0xE0, 0x04, 0x00, 0x31, 0x32, 0x80, 0xAF, 0x0D, 0x00, 0x06, 0x60, 0x40, 0x00, 0x81, 0x00, 0x04, 0xD0};
// uint8_t off_state[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02, 0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x0D, 0x00, 0x06, 0x60, 0x40, 0x00, 0x81, 0x00, 0x04, 0xCF};

uint8_t convert_state[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02, 0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x0D, 0x00, 0x06, 0x60, 0x40, 0x00, 0x81, 0x00, 0x04, 0xCF};


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

bool checkState()
{
  http.begin("testing10293.herokuapp.com", 80, "/check");
    int httpCode = http.GET();
    if(httpCode > 0) {
        // 在序列埠監控視窗顯示送出的數據
        Serial.printf("HTTP code: %d\n", httpCode);
        
        if(httpCode == 200) {
          String payload = http.getString();
          // String payload = "{\"status\":0,\"state\":[2,32,224,4,0,0,0,6,2,32,224,4,0,49,52,128,175,13,0,6,96,64,0,129,0,4,210]}";
          // 顯示遠端伺服器的回應
          Serial.println(payload);
          const char *cstr = payload.c_str();
          // Serial.println(cstr);
          error = deserializeJson(doc, cstr);
          int dd = doc["status"];
          print(dd)
          if(dd == 1)
          {
            for (int i =0;i<27;++i)
            {
              uint8_t s = doc["state"][i];
              convert_state[i] = s;
              Serial.println(s);
            }
            return 1;
          }
          else{
            return 0;
          }
        }
    } else {
        Serial.println("HTTP connection ERROR!");
        return -1;
    }
    
}

void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   pinMode(BTN_PIN, INPUT_PULLUP); 
  irsend.begin();
  // Serial.begin(115200);

  #if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  #else  // ESP8266
    Serial.begin(115200, SERIAL_8N1);
  #endif  // ESP8266

  WiFi.begin(ssid, pass);
  
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi ready...");
  // line();
}

void loop() {
  Serial.print("Checking");
  bool recv_state = checkState();
  if(recv_state){
    Serial.println("Get Signal");
    irsend.sendPanasonicAC(convert_state);
    delay(2000);
  }
  // if(recv_state=="on"){
  //   Serial.println("[turn on] a Panasonic A/C state from IRrecvDumpV2");
  //   irsend.sendPanasonicAC(on_state);
  //   delay(2000);
  // }else if(recv_state=="off"){
  //   Serial.println("[turn off] a Panasonic A/C state from IRrecvDumpV2");
  //   irsend.sendPanasonicAC(off_state);
  //   delay(2000);
  // }
  Serial.print("end");
  delay(2000);
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
    // delay(2000);
 }