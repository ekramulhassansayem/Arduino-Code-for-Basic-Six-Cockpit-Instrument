/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <LCD.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal.h>
LiquidCrystal_SR lcd(6, 4, 5);

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

    float data[7];
    float pr;
    float te;
    float al;
    float x;
    float y;
    float z;
    float pr1;

    int button =A0;
    int value = 0;
    int s=0;
    
    
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows: 
  lcd.clear(); 
  
 
  
 
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}
void loop() {
    
    
    while (!radio.available());
    radio.read(data, sizeof(data)); 
    pr=data[0];
    te=data[1];
    al=data[2];
    x=data[3];
    y=data[4];
    z=data[5];
    pr1=data[6];
    

    value=analogRead(button);
    {
  
if (value>=470 &&value<=485){
    s=1;
  }
  else if (value>=120&&value<=130){
    s=2;
  }
  else if (value>=920 &&value<=940){
    s=3;
  }
  else if (value>=165 &&value<=180){
    s=4;
  }
  else if (value>= 300&&value<=320){
    s=5;
  }
  else if (value>=500 &&value<=520){
    s=6;
  }
    }
    
  
    
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("WELCOME TO BASIC"); // print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print("SIX INSTRUMENT:"); // print a text
    




  if(s==1)
  {
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("Total :");
  lcd.print(pr1);// print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print("Static:");
  lcd.print(pr);// print a text
  
  }
  if(s==2)
  {
    
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("PITCH:");
  lcd.print(x);
  lcd.print("degree");// print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print("ROLL:");
  lcd.print(y);
  lcd.print("degree");// print a text
  
  
  }
  if(s==3)
  {
    lcd.setCursor(0,0); // put cursor at colon 0 and row 0
  lcd.print("ALTIMETER:      "); // print a text
  lcd.setCursor(0,1); // put cursor at colon 0 and row 1
  lcd.print(al); // print a text
  lcd.print("m  ");
  lcd.print(al*3.28084);
  lcd.print("ft");
  }
  if(s==4)
  {
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("TURN INDICATOR:"); // print a text
  lcd.setCursor(0, 1);
  if(y>0){// put cursor at colon 0 and row 1
  lcd.print("RIGHT TURN "); // print a text
  lcd.print(y);
  lcd.print("  ");
  }
  else{
    lcd.print("LEFT TURN ");
    lcd.print(y);
    lcd.print("  ");
    }
  }
  if(s==5)
  {
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("HEADING INDICATR:"); // print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print(z); // print a text
  if(z>=70&&z<=110){
    lcd.print(" WEST     ");      
    }
  if(z>20&&z<70){
    lcd.print(" SW       ");      
    }
    if(z>110&&z<160){
    lcd.print(" NW      ");      
    }
    if(z>=160&&z<=200){
    lcd.print(" NORTH      ");      
    }
    if(z>200&&z<250){
    lcd.print(" NE      ");      
    }
    if(z>=250&&z<=290){
    lcd.print(" EAST      ");      
    }
    if(z>290&&z<340){
    lcd.print(" SE      ");      
    }
    if((z>=340)||(z<=20)){
    lcd.print(" SOUTH      ");      
    }
    
  
  }
  if(s==6)
  {
    lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("VR SPEED INDICATOR:"); // print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print("Static pr:"); // print a text
  
  }

   
}
