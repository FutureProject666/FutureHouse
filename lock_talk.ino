#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid      = "FutureHouse";
const char* password  = "nlhs7626";
#define FIREBASE_HOST "https://futurehouse-7626.firebaseio.com/"
#define FIREBASE_AUTH "gSN7Uu3uxmCEUrlZVINHIezEsVdM996Ua6ugNzW2"


#define REC D2 // pin 2 is used for recording
#define PLAY_E D3 // pin 3 is used for playback-edge trigger
// when PLAY_E goes HIGH device will playbak STARTS and continues
// ISD1820 Arduino Voice Recorder for Robojax.com

#define PLAY_L D4 // pin 4 is used for playback  
// when PLAY_L is HIGH it will playback. As soon as it goes LOW playback STOPS
// So you have to keep it HIGH to keep playing


#define FT D5 // pin 5 is used for feed through
// if the SPI (speaker) pin is used to send Audio to other device,
// set FT to high and any audio from microphone will pass through the SPI connector
// and will NOT record

#define playTime 5000 // playback time 5 seconds
#define recordTime 3000 // recording time 3 seconds
#define playLTime 900 // press and release playback time 0.9 seconds

String character;
String content;
String last;
String Last;
FirebaseData firebaseData;

void printJsonObjectContent(FirebaseData &data);

void setup()
{
  // ISD1820 Arduino Voice Recorder for Robojax.com
  pinMode(REC, OUTPUT); // set the REC pin as output
  pinMode(PLAY_L, OUTPUT); // set the PLAY_L pin as output
  pinMode(PLAY_E, OUTPUT); // set the PLAY_e pin as output
  pinMode(FT, OUTPUT); // set the FT pin as output
  Serial.begin(9600);// set up Serial monitor
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop() {
  // ISD1820 Arduino Voice Recorder for Robojax.com
  String path = "/house"; //資料庫分頁名

  FirebaseJson json;

  //  while (Serial.available() > 0) {
  //    char inChar = (char)Serial.read();
  //    if (inChar == 'p' || inChar == 'P') {
  //      digitalWrite(PLAY_E, HIGH);
  //      delay(50);
  //      digitalWrite(PLAY_E, LOW);
  //      Serial.println("Playbak Started");
  //      delay(playTime);
  //
  //      Serial.println("Playbak Ended");
  //      break;
  //    }// if
  //    else if (inChar == 'r' || inChar == 'R') {
  //      digitalWrite(REC, HIGH);
  //      Serial.println("Recording started");
  //      delay(recordTime);
  //      digitalWrite(REC, LOW);
  //      Serial.println("Recording Stopped ");
  //    }
  //
  //  }
  //
  //  delay(500);


  if (Firebase.getString(firebaseData, path + "/gate"))
  {
    Serial.print("lcdprint: ");
    Serial.println(firebaseData.stringData());

    content = firebaseData.stringData();
    
    Serial.println("------------------------------------");
    Serial.println();
    if(Last!=content){
    if (content == "1")
    {
      Serial.println("Playbak Started");
      digitalWrite(PLAY_E, HIGH);
      delay(5000);
      digitalWrite(PLAY_E, LOW);
      Serial.println("Playbak Ended");  
    }
    
      
    
  }

  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  }

  if (Firebase.getString(firebaseData, path + "/talk"))
  {
    Serial.print("talk: ");
    Serial.println(firebaseData.stringData());
    character = firebaseData.stringData();
    if (last != character) {
      Serial.println("------------------------------------");
      Serial.println();
      Serial.print("last: ");
      Serial.println(last);
      Serial.print("character: ");
      Serial.println(character);
      if (character == "2")
      {
        digitalWrite(REC, HIGH);
        Serial.println("Recording started");
      }
      if (content == "0" ) {
        digitalWrite(REC, LOW);
        Serial.println("Recording Stopped ");
      }
      else
      {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }
    }



  }


  last = character;
  Last = content;
}


void printJsonObjectContent(FirebaseData &data) {
  size_t tokenCount = data.jsonObject().parse(false).getJsonObjectIteratorCount();
  String key;
  String value;
  FirebaseJsonObject jsonParseResult;
  Serial.println();
  for (size_t i = 0; i < tokenCount; i++)
  {
    data.jsonObject().jsonObjectiterator(i, key, value);
    jsonParseResult = data.jsonObject().parseResult();
    Serial.print("KEY: ");
    Serial.print(key);
    Serial.print(", ");
    Serial.print("VALUE: ");
    Serial.print(value);
    Serial.print(", ");
    Serial.print("TYPE: ");
    Serial.println(jsonParseResult.type);

  }
}
