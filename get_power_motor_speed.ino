#include<Servo.h>
#include<time.h>

Servo power;
void setup() {
  // put your setup code here, to run once:
  power.attach(A0);
  Serial.begin(9600);
  Serial.println("Time start");
  float t1 = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  power.write(30);
  if (Serial.available() >= 1){
     float t2 = millis();
     Serial.println("Got time!!");
     Serial.println(t2-t1);  
  }
  
}
