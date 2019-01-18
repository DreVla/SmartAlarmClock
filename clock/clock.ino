
// include the library code:
#include <LiquidCrystal.h>
int tempPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
int photocellPin = 1;
int photocellReading;
int tempReading;
int LEDpin = 10;
int LEDbrightness;
int redPin= 8;
int greenPin = 6;
int bluePin = 7;
int stopAlarmButton = 13;
int alarm_hour = 7, alarm_minutes = 0;
int set_hour = 4;
int set_minutes = 5;
int buzzer = 9;
int mode1, mode2, t=2, f=0, second=05, minutes=58, hours=03;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600); 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 1);
  //lcd.print("Temp:");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(stopAlarmButton, INPUT);
}
int stopAlarm = 0;
int stopAlarmCheck = 0;
void loop() {
  lcd.clear();
  lcd.setCursor(11,1);
  lcd.print(alarm_hour);
  lcd.print(":");
  lcd.print(alarm_minutes);
  stopAlarm = digitalRead(stopAlarmButton);
  if(stopAlarm == HIGH) stopAlarmCheck = 1;
  
  // read temperature from temp Sensor ---------------------------
  tempReading = analogRead(tempPin); 
  // read light intensity from photocell -------------------------
  photocellReading = analogRead(photocellPin);
  Serial.println(photocellReading);
  photocellReading = 1023 - photocellReading;
  // depending on light intensity we give more brightness to the led -------------
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  analogWrite(LEDpin, LEDbrightness);
  // read temperature from temp Sensor ---------------------------
  tempReading = analogRead(tempPin); 
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = tempReading * 5.0;
  voltage /= 1024.0; 
  Serial.print(voltage);
  Serial.println(" volts");
  int tempReading = analogRead(tempPin);  
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
  Serial.print(temperatureC);
  Serial.println(" C");
  lcd.setCursor(12,0);
  lcd.print(temperatureC, 0);
  lcd.print("C");
  if(temperatureC > 20 && temperatureC < 24){
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
  } else if(temperatureC < 20){
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
  }else { 
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
  }

  // here comes the clock part ------------------------------
  lcd.setCursor(0, 0);
  lcd.print("18/Jan/2019"); // print out the date
  
  f=t%2;
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  lcd.setCursor(0,1);
  second++;
  if(second>=60)
  {
    second=0;
    minutes++;
  }
  if(minutes>=60)
  {
    second=0;
    minutes=0;
    hours++;
  }
  if(hours>=13)
  {
    second=0;
    minutes=0;
    hours=1;
    t++;
  }
  
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(second);
  if(f==0)
  {
    lcd.print(" PM");
  }
  else
  {
    lcd.print(" AM");
  }
  if(mode1==1)
  {
    delay(100);
    minutes++;
  }
  if(mode2==1)
  {
    delay(1000);
    hours++;
  }
  else
  {
    delay(1000);
  }

  stopAlarm = digitalRead(stopAlarmButton);
  if(stopAlarm == HIGH) alarm_hour++;
  if(hours == alarm_hour && minutes == alarm_minutes && stopAlarmCheck == 0){
  tone(buzzer, 1000, 500);
  }

}
