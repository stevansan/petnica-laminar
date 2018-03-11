#include <ACPWM.h>
#include <Wire.h>

#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);

#define ZERO_PIN 2 //Input pin from zero cross detector
 #define PWM_PIN 9 //Output pin to TRIAC / SSR
 int SET_PIN = A4; //Analog pin for setting the dutyCycle value with a pontentiometer


//const int rs = 2, en = 3, d4 = 5, d5 = 4, d6 = 6, d7 = 7;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Enkoder i njegov taster
int val;
int encoder0PinA = 8;
int encoder0PinB = 3;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int dugmePin = 10;
int dugmeState = 0;
int stanje = 0;

// UV relej i motor prekidac
int UVPout = A2 ;
int motorPin = A3;
int motorStanje = 0;
int c = 0;

void setup()
{

  // Pozdravna poruka
  lcd.begin(16, 2);
  lcd.print("     Welcome to   ");
  lcd.setCursor(0,1);
  lcd.print("  Utepuvatch V1.0  ");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Made by");
  lcd.setCursor(0,1);
  lcd.print("Aleksa and Stevan");
  delay(2000);
  lcd.clear();
  lcd.print("UV timer:");
  lcd.setCursor(0,1);
  lcd.print("Press to start");
  lcd.setCursor(15,0);
  lcd.print("min");
  lcd.setCursor(10,0);
  lcd.print("0");

  // Enkoder pinovi
  pinMode(dugmePin, INPUT);
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (UVPout, OUTPUT);

  // GORE/DOLE
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);

  // Motor dugme
  pinMode(motorPin, INPUT);
  digitalWrite(motorPin, HIGH);


  // ACPWM setup

   pinMode(ZERO_PIN, INPUT);
   pinMode(PWM_PIN, OUTPUT);
    //Initialize PWM operation.
    //Mains frequency: 50Hz.
    //Zero crossing point reached whenever pulse to PIN2 changes
    //Duty cycle = 0..255. 0:always off. 255: always on. 150: 59% on.
    ACpwm.initialize(50,ZERO_PIN,CHANGE,PWM_PIN,255);
    //Latching on HIGH zero state: 3 microseconds.
    //Latching on LOW zero cross state: 5 microseconds.
    ACpwm.setLatch(3,5);

}


// Pocetak programa

void loop()
{



  // PWM

  motorStanje = digitalRead(motorPin);
  if (motorStanje == LOW)
  {
    motorStanje = 10;
  }
  else
  {
    c = 0;
    ACpwm.setDutyCycle(0);
  }

  // Kod koji se izvrsava kada se pritisne taster enkodera
   if (motorStanje == 10 || c < 1024 )
  {
    c++;
    delay(100);
      ACpwm.setDutyCycle(c);
  }



  //Ispisivanje poruke
  lcd.setCursor(0,1);
  lcd.print("Press to start");


if (digitalRead(12) == LOW  && digitalRead(13) == LOW)   // Stoji
{
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
}

if (digitalRead(12) == HIGH  && digitalRead(13) == HIGH)   // Stoji
{
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
}

if (digitalRead(12) == HIGH  && digitalRead(13) == LOW)  // Ide dole
{
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
}


if (digitalRead(12) == LOW  && digitalRead(13) == HIGH)   // Ide gore
{
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
}



  // kod za enkoder
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH))
  {
    if (digitalRead(encoder0PinB) == LOW)
    {
      encoder0Pos--;
    } else
    {
      encoder0Pos++;
    }
    if (encoder0Pos < 0)
    {
      encoder0Pos = 0;
    }
    delay(80);
    lcd.setCursor(10,0);
    lcd.print (encoder0Pos);
    lcd.print("  ");

  }
  encoder0PinALast = n;


  // Latch za taster enkodera
  dugmeState = digitalRead(dugmePin);
  if (dugmeState == HIGH)
  {
    stanje = 10;
  }

  // Kod koji se izvrsava kada se pritisne taster enkodera
   if (stanje == 10)
  {
    c = 0;
    digitalWrite(UVPout, HIGH);
    lcd.setCursor(10,0);
    lcd.print("    ");
    lcd.setCursor(0,1);
    lcd.print("Bakterije na plazi!");

    if (encoder0Pos > 0)
    {
      encoder0Pos --;
      lcd.setCursor(10,0);
      lcd.print(encoder0Pos);


      delay(600);
      }
   else
    {
    digitalWrite(UVPout, LOW);
    lcd.setCursor(10,0);
    lcd.print("0");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("Done");
    delay(2000);
    lcd.setCursor(0,1);
    encoder0Pos = 0;
    stanje = 0;
    }
   }



  }
