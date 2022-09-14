#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "MM5rB5yKr2DYsn7g_QEZjqsRda45Yk7z";//Enter your Auth token
char ssid[] = "Varsha Patel";//Enter your WIFI name
char pass[] = "varsha patel";//Enter your WIFI password
BlynkTimer timer;
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Servo.h>

#define ir1 D6
#define sen1 A0 //moisture
//#define buzzer 2
#define trigPin1 D4
#define echoPin1 D5
#define trigPin2 D7
#define echoPin2 D8 

long drysensor, wetsensor;
// defines variables
long duration1, duration2; // variable for the duration of sound wave travel
int distance1;
int distance2; // variable for the distance measurement
int level1=0;
int level2=0;
int mc;
int onetime=0;
int onetime1=0;
int wet=0,moisture=0,object=0,cabin2=0,c1=0,c2=0;
Servo myservo;

void setup()
{
  Serial.begin (9600);
  lcd.init();// initialize the LCD
  lcd.clear();
  lcd.backlight();// Turn on the blacklight and print a message.
  lcd.setCursor(0,0);
  lcd.print("Automatic Waste");
  lcd.setCursor(0,1);
  lcd.print("Segregation System");
  delay(1000);
  pinMode(ir1,INPUT);
  pinMode(sen1,INPUT);
  //pinMode(buzzer,OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  myservo.attach(D0);
  myservo.write(90);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  delay(500);
  lcd.clear();
  lcd.print("Sensing Garbage");
  delay(1000);
  lcd.clear();
  
  //Call the functions
  timer.setInterval(100L, dryultasonicsensor);
  timer.setInterval(100L, wetultasonicsensor);

}

void loop()
{
  Blynk.run();
  timer.run();
  moisture=analogRead(sen1);
 //Serial.print("moisture = ");
  //Serial.println(moisture);
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DRY-%   WET-%");
  lcd.setCursor(1,1);
  lcd.print(level1);
  lcd.setCursor(10,1);
  lcd.print(level2);
  delay(1000);

  object=digitalRead(ir1);
  moisture=analogRead(sen1);
  if(object==LOW)
  {
    lcd.clear();
    moisture=analogRead(sen1);
    lcd.setCursor(0,0);
    lcd.print("GARBAGE DETECTED");
    lcd.setCursor(0,1);
    lcd.print("moisture: ");
    lcd.setCursor(12,1);
    lcd.print(moisture);
    //digitalWrite(buzzer, HIGH);
    delay(150);
    //digitalWrite(buzzer, LOW);
    delay(1000);
    moisture=analogRead(sen1);
    delay(500);
    if(moisture>=50)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("WET GARBAGE");
      myservo.write(0);
      delay(5000);
      myservo.write(90);
      delay(2000);
      Serial.print("wet");
    }
    else if(moisture<40)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("DRY GARBAGE");
      myservo.write(180);
      delay(5000);
      myservo.write(90);
      delay(2000);
      Serial.print("dry");
      lcd.clear();
    }
  }
}

void dryultasonicsensor()
{
  digitalWrite(trigPin1, LOW); // Clears the trigPin condition
  delayMicroseconds(2);
  
  digitalWrite(trigPin1, HIGH); // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  delayMicroseconds(100);
  digitalWrite(trigPin1, LOW);
  
  duration1 = pulseIn(echoPin1, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance1 = duration1 / 29 / 2; // Speed of sound wave divided by 2 (go and back) // Calculating the distance
  level1 = 100-(distance1*100/18);
  Blynk.virtualWrite(V4, level1);
  
}


void wetultasonicsensor()
{
  digitalWrite(trigPin2, LOW); // Clears the trigPin condition
  delayMicroseconds(2);
  
  digitalWrite(trigPin2, HIGH); // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  delayMicroseconds(100);
  digitalWrite(trigPin2, LOW);
  
  duration2 = pulseIn(echoPin2, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance2 = duration2 / 29/ 2; // Speed of sound wave divided by 2 (go and back) // Calculating the distance
  level2 = 100-(distance2*100/18);
  Blynk.virtualWrite(V5, level2);
}
