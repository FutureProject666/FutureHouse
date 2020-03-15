#include <SoftwareSerial.h>
#define LED_PIN 13

SoftwareSerial fromESP(7, 8); //Rx, Tx

char inChar;
int swit = 1;
void setup()
{
  Serial.begin(115200);
  fromESP.begin(9600);
  delay(50);
  pinMode(LED_PIN, OUTPUT);
  delay(50);
  digitalWrite(LED_PIN, HIGH);
}

void loop()
{
  if (fromESP.available())
  {
    inChar = fromESP.read();
    Serial.print(inChar);
  }
  if (inChar == '0') {
    if (swit == 1) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("更換為關閉");
      swit = 0;
    }
    Serial.println("已經為關閉");
  }
  if (inChar == '1') {
    if (swit == 0) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("更換為開啟");
      swit = 1;
    }
    Serial.println("已經為開啟");
  }

}
