#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL9VQ2XyDK"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "ctJnNVEn7PyeGf-3R4qI0EQtzSYXf5xK"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>

using namespace ace_button;

const int BUTTON_PIN = 2;
const int LIGHT_PIN = 15;
const int FAN_PIN = 13;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Binh Minh";
char pass[] = "0979906430";
int state = 0;

AceButton button(BUTTON_PIN);

void handleEvent(AceButton*, uint8_t, uint8_t);

void setup()
{
  pinMode(LIGHT_PIN,OUTPUT);
  pinMode(FAN_PIN,OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V0){
  if(param.asInt() == 1){
    digitalWrite(LIGHT_PIN,HIGH);
  }
  else
  {
    digitalWrite(LIGHT_PIN,LOW); 
  }
}

BLYNK_WRITE(V1){
  if(param.asInt() == 1){
    digitalWrite(FAN_PIN,HIGH);
    Serial.print(digitalRead(FAN_PIN));
    state=1;
  }
  else
  {
    digitalWrite(FAN_PIN,LOW);
        Serial.print(digitalRead(FAN_PIN));
    state=0;
  }
}

BLYNK_WRITE(V3){
  if(state==1){
    TimeInputParam t(param);
    unsigned int startseconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);  
    unsigned int stopseconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60);
    unsigned int duration = stopseconds - startseconds;
    Serial.println("Start: "+String(startseconds));
    Serial.println("Stop: "+String(stopseconds));
    Serial.println("Duration in miliseconds: "+String(duration*1000)); 
    delay(duration*1000);
    Blynk.virtualWrite(V1, 0);
    digitalWrite(FAN_PIN,LOW);
  }
}

void handleEvent(AceButton* /*button*/, uint8_t eventType, uint8_t /*buttonState*/) {
  switch (eventType) {
    case AceButton::kEventPressed:
      if(digitalRead(LIGHT_PIN)){
        digitalWrite(LIGHT_PIN,LOW);
        Blynk.virtualWrite(V0, 0);
      }else{
        digitalWrite(LIGHT_PIN,HIGH);
        Blynk.virtualWrite(V0, 1);
      }
      break;
    case AceButton::kEventReleased:
      if(digitalRead(LIGHT_PIN)){
        digitalWrite(LIGHT_PIN,LOW);
        Blynk.virtualWrite(V0, 0);
      }else{
        digitalWrite(LIGHT_PIN,HIGH);
        Blynk.virtualWrite(V0, 1);
      }
      break;
  }
}

void loop()
{
  Blynk.run();
  button.check();
}
