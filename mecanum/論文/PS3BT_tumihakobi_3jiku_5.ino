#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif



#include "NAxisMotion.h"
#include <Wire.h>

int M1 = 8;             //メカナムホイール用
int M2 = 11;         //3軸ジャイロに4ピンは使えない
int M3 = 12;
int M4 = 3;
int E1 = 22;
int Q1 = 26;
int E2 = 23;
int Q2 = 27;
int E3 = 24;
int Q3 = 28;
int E4 = 32;
int Q4 = 33;
int LeftY = 0;
int LeftX = 0;
int RightX = 0;
int vector1 = 0;
int vector2 = 0;
int vector3 = 0;
int vector4 = 0;
int absolute1 = 0;
int absolute2 = 0;
int absolute3 = 0;
int absolute4 = 0;
int range1 = 0;
int range2 = 0;
int range3 = 0;
int range4 = 0;
int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;




int Rf=180;
int rf=0;
int sp=0;
int err=0;
int err2=0;




USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);



//3軸ジャイロ
NAxisMotion mySensor;
unsigned long lastStreamTime = 0;
const int streamPeriod = 20;





void setup() { 


  pinMode(E1, OUTPUT);                            //メカナムホイール用入出力
  pinMode(Q1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(Q2, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(Q3, OUTPUT);
  pinMode(E4, OUTPUT);
  pinMode(Q4, OUTPUT);
  analogWrite(M1, value1);
  analogWrite(M2, value2);
  analogWrite(M3, value3);
  analogWrite(M4, value4);


  
 Serial.begin(115200);
  while (!Serial);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }  
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  
  
  
  
   I2C.begin();
  //Sensor Initialization
  mySensor.initSensor();
  mySensor.setOperationMode(OPERATION_MODE_NDOF);
  mySensor.setUpdateMode(MANUAL);
 
  
  
  
  
  
}


void loop() {
  Usb.Task();
  
//メカナムホイールプログラム開始

  static int mokuhyouti = 0;
  static int kaiten = 0;
  static int RS0 = 0;
  int RS = (float)(mySensor.readEulerHeading());
  if(RS > 360-20 && RS0 < 0+20) {
    kaiten = kaiten - 1;
  }
  if(RS0 > 360-20 && RS < 0+20) {
    kaiten = kaiten + 1;
  }
  rf = 360 * kaiten + RS;
  RS0 = RS;
  
  if (PS3.PS3Connected){
    if (PS3.getAnalogHat(LeftHatY) > 135 || PS3.getAnalogHat(LeftHatY) < 120){
      LeftY = (int)((((float)PS3.getAnalogHat(LeftHatY) - 127.5f) / 127.5f) * -255.0f);
    }
    else{
      LeftY = 0;
    }

    
    if (PS3.getAnalogHat(LeftHatX) > 135 || PS3.getAnalogHat(LeftHatX) < 120){
      LeftX = (int)((((float)PS3.getAnalogHat(LeftHatX) - 127.5f) / 127.5f) * 255.0f);
    }
    else{
      LeftX = 0;
    }
    if (PS3.getAnalogHat(RightHatX) > 135 || PS3.getAnalogHat(RightHatX) < 120){
      RightX = (int)((((float)PS3.getAnalogHat(RightHatX) - 127.5f) / 127.5f) * 255.0f);
    }
    else{
      
      mokuhyouti = rf;
      RightX = 20 * (0 - rf);
      RightX = constrain(RightX, -20, 20);
    //Serial.print(rf);
    //Serial.print(' ');
    Serial.println(RightX);
    }
    if (LeftY != 0 || LeftX != 0 || RightX != 0){
      vector1 = LeftY + LeftX + RightX;
      vector2 = -(LeftY - LeftX - RightX);
      vector3 = LeftY - LeftX + RightX;
      vector4 = -(LeftY + LeftX - RightX);
      absolute1 = abs(vector1);
      absolute2 = abs(vector2);
      absolute3 = abs(vector3);
      absolute4 = abs(vector4);
      range1 = constrain(absolute1, 0, 255);
      range2 = constrain(absolute2, 0, 255);
      range3 = constrain(absolute3, 0, 255);
      range4 = constrain(absolute4, 0, 255);
      value1 = range1 * (vector1 / absolute1);
      value2 = range2 * (vector2 / absolute2);
      value3 = range3 * (vector3 / absolute3);
      value4 = range4 * (vector4 / absolute4);
    }
    else{
      value1 = 0;
      value2 = 0;
      value3 = 0;
      value4 = 0;
    }



    if (value1 > 0){
      digitalWrite(E1,HIGH);
      digitalWrite(Q1,LOW);
    }
    else if (value1 < 0){
      digitalWrite(E1,LOW);
      digitalWrite(Q1,HIGH);
      value1 = abs(value1);
    }
    else {
      digitalWrite(E1,LOW);
      digitalWrite(Q1,LOW);
    }

    if (value2 > 0){
      digitalWrite(E2,HIGH);
      digitalWrite(Q2,LOW);
    }
    else if (value2 < 0){
      digitalWrite(E2,LOW);
      digitalWrite(Q2,HIGH);
      value2 = abs(value2);
    }
    else {
      digitalWrite(E2,LOW);
      digitalWrite(Q2,LOW);
    }

    if (value3 > 0){
      digitalWrite(E3,HIGH);
      digitalWrite(Q3,LOW);
    }
    else if (value3 < 0){
      digitalWrite(E3,LOW);
      digitalWrite(Q3,HIGH);
      value3 = abs(value3);
    }
    else {
      digitalWrite(E3,LOW);
      digitalWrite(Q3,LOW);
    }

    if (value4 > 0){
      digitalWrite(E4,HIGH);
      digitalWrite(Q4,LOW);
    }
    else if (value4 < 0){
      digitalWrite(E4,LOW);
      digitalWrite(Q4,HIGH);
      value4 = abs(value4);
    }
    else {
      digitalWrite(E4,LOW);
      digitalWrite(Q4,LOW);
    }
    
    analogWrite(M1, value1);
    analogWrite(M2, value2);
    analogWrite(M3, value3);
    analogWrite(M4, value4);

    if (PS3.getButtonClick(PS)){
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    
  }

//ここまでメカナムホイール
  

    if (PS3.getButtonClick(PS)) {                       //PSボタン
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }

 
 
 
   if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();    
    mySensor.updateEuler();
    mySensor.updateCalibStatus();
  }
  
  
       if (PS3.getButtonClick(CIRCLE)) {                 //○ボタン
        Serial.print(F("\r\nCircle"));
 //       (mySensor.readEulerHeading())=0
      } 

 
}   

      
    
