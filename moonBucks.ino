#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
String inputString = "";

Servo servo1;
Servo servo2;
Servo servo3;

#define ONE_WIRE_BUS 9
#define Relay 10
#define pump1 11
#define servo_1_pin 22
#define servo_2_pin 24
#define servo_3_pin 26
#define IR1 23
#define IR2 27
#define trig1 44
#define echo1 45
#define trig2 48
#define echo2 49
#define trig3 52
#define echo3 53
#define buzzer 12


char c = '8';
int distance = 0, t1 = 0, t2 = 0, t3 = 0;
int ultraSonic1 = 0, ultraSonic2 = 0, ultraSonic3 = 0;

int IR1Value;
int IR2Value;

float myTemp;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;


char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 16, 2);

int small = 50;
int medium = 70;

char key;
bool IR1_flag;
bool IR2_flag;

void lcd_screen_init() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("0#Normal/1#NoSug");
  lcd.setCursor(0, 1);
  lcd.print("2#Sugar+/3#Dark");

}

void closeMotors() {
  servo1.write(23);
  servo2.write(10);
  servo3.write(20);
}

void buzzerOff() {
  digitalWrite(buzzer, LOW);
}

void buzzerOn() {
  digitalWrite(buzzer, HIGH);
  delay(400);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(400);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(400);
  digitalWrite(buzzer, LOW);
  delay(200);
}


void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  inputString.reserve(1);
  lcd_screen_init();
  sensors.begin();

  closeMotors();

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  servo1.attach (servo_1_pin); // shuger
  servo2.attach(servo_2_pin); // niskafae
  servo3.attach(servo_3_pin); // coffee mate

  pinMode(pump1, OUTPUT);
  digitalWrite(pump1, HIGH);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  IR1_flag = true;
  IR2_flag = true;

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);

  pinMode(buzzer, OUTPUT);

}


void ultraSonicFunction() {

  digitalWrite(trig1, LOW);
  digitalWrite(trig2, LOW);
  digitalWrite(trig3, LOW);
  delayMicroseconds(5);
  digitalWrite(trig1, HIGH);
  digitalWrite(trig2, HIGH);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig1, LOW);
  t1 = pulseIn(echo1, HIGH);
  ultraSonic1 = t1 / 57;

  digitalWrite(trig2, LOW);
  t2 = pulseIn(echo2, HIGH);
  ultraSonic2 = t2 / 57;

  digitalWrite(trig3, LOW);
  t3 = pulseIn(echo3, HIGH);
  ultraSonic3 = t3 / 57;

}

void normal_Nes_Motors(int quantity) {
  int tmp = quantity;

  while (tmp) {
    servo1.write(tmp);
    delay(15);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo2.write(tmp);
    delay(30);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo3.write(tmp);
    delay(10);
    tmp--;
  }
}

void noSugar_Nes_Motors(int quantity) {
  int tmp = quantity;

  while (tmp) {
    servo1.write(tmp);
    delay(15);
    tmp--;
  }

  tmp = quantity;
  while (tmp) {
    servo3.write(tmp);
    delay(10);
    tmp--;
  }
}
void sugarPlus_Nes_Motors(int quantity) {
  int tmp = quantity;

  while (tmp) {
    servo1.write(tmp);
    delay(15);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo2.write(tmp);
    delay(40);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo3.write(tmp);
    delay(10);
    tmp--;
  }
}

void dark_Nes_Motors(int quantity) {
  int tmp = quantity;

  while (tmp) {
    servo1.write(tmp);
    delay(17);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo2.write(tmp);
    delay(30);
    tmp--;
  }
  tmp = quantity;
  while (tmp) {
    servo3.write(tmp);
    delay(8);
    tmp--;
  }
}

void normal() {
  if (IR1Value == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put a cup first!");
    delay(2500);
    lcd_screen_init();
  }
  else {
    if (ultraSonic1 == 9 || ultraSonic2 == 11 || ultraSonic3 >= 12) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fill the empty");
      delay(1500);
      lcd_screen_init();
    }
    else  {
      // closeMotors();
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println(inputString);
      lcd.print("0#");
      lcd.setCursor(1, 0);
      Serial.println(key);
      lcd.write(key);

      lcd.setCursor(0, 1);
      Serial.println("Normal");
      lcd.print("Normal");
      IR1_flag = true;
      IR2_flag = true;
      inputString = "";
      if (IR1Value == 0 && IR1_flag == true && IR2Value == 1) {
        normal_Nes_Motors(small);

        IR1_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(2800);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      else  if (IR2Value == 0 && IR2_flag == true && IR1Value == 0 && IR1_flag == true) {
        normal_Nes_Motors(medium);
        IR1_flag = false;
        IR2_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(3500);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      buzzerOn();
    }
  }
}

void noSugar() {
  if (IR1Value == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put a cup first!");
    delay(2500);
    lcd_screen_init();
  }
  else {
    if (ultraSonic1 == 9 || ultraSonic2 == 11 || ultraSonic3 >= 12) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fill the empty");
      delay(1500);
      lcd_screen_init();
    }
    else {
      // closeMotors();
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println(inputString);
      lcd.print("1#");
      lcd.setCursor(1, 0);
      Serial.println(key);
      lcd.write(key);

      lcd.setCursor(0, 1);
      Serial.println("No Sugar");
      lcd.print("No Sugar");
      IR1_flag = true;
      IR2_flag = true;
      inputString = "";
      if (IR1Value == 0 && IR1_flag == true && IR2Value == 1) {
        noSugar_Nes_Motors(small);

        closeMotors();
        digitalWrite(pump1, LOW);
        delay(2800);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      else  if (IR2Value == 0 && IR2_flag == true && IR1Value == 0 && IR1_flag == true) {
        noSugar_Nes_Motors(medium);
        IR1_flag = false;
        IR2_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(3500);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      buzzerOn();
    }
  }
}


void sugarPlus() {
  if (IR1Value == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put a cup first!");
    delay(2500);
    lcd_screen_init();
  }
  else {
    if (ultraSonic1 == 9 || ultraSonic2 == 11 || ultraSonic3 >= 12) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fill the empty");
      delay(1500);
      lcd_screen_init();
    }
    else {
      // closeMotors();
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println(inputString);
      lcd.print("2#");
      lcd.setCursor(1, 0);
      Serial.println(key);
      lcd.write(key);

      lcd.setCursor(0, 1);
      Serial.println("Sugar Plus");
      lcd.print("Sugar Plus");
      IR1_flag = true;
      IR2_flag = true;
      inputString = "";
      if (IR1Value == 0 && IR1_flag == true && IR2Value == 1) {
        sugarPlus_Nes_Motors(small);
        IR1_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(2800);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      else  if (IR2Value == 0 && IR2_flag == true && IR1Value == 0 && IR1_flag == true) {
        sugarPlus_Nes_Motors(medium);
        IR1_flag = false;
        IR2_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(3500);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      buzzerOn();
    }
  }
}

void dark() {
  if (IR1Value == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put a cup first!");
    delay(2500);
    lcd_screen_init();
  }
  else {
    if (ultraSonic1 == 9 || ultraSonic2 == 11 || ultraSonic3 >= 12) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fill the empty");
      delay(1500);
      lcd_screen_init();
    }
    else {
      //closeMotors();
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println(inputString);
      lcd.print("3#");
      lcd.setCursor(1, 0);
      Serial.println(key);
      lcd.write(key);

      lcd.setCursor(0, 1);
      Serial.println("Dark");
      lcd.print("Dark");
      IR1_flag = true;
      IR2_flag = true;
      inputString = "";
      if (IR1Value == 0 && IR1_flag == true && IR2Value == 1) {
        dark_Nes_Motors(small);
        IR1_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(2800);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      else  if (IR2Value == 0 && IR2_flag == true && IR1Value == 0 && IR1_flag == true) {
        dark_Nes_Motors(medium);
        IR1_flag = false;
        IR2_flag = false;
        closeMotors();
        digitalWrite(pump1, LOW);
        delay(3500);
        digitalWrite(pump1, HIGH);
        lcd_screen_init();
      }
      buzzerOn();
    }
  }
}


void loop() {
  key = keypad.getKey();
  Serial.println("key is ");
  Serial.println(key);

  IR1Value = digitalRead(IR1);
  IR2Value = digitalRead(IR2);

  Serial.println("IR -->");
  Serial.println(IR1Value);
  Serial.println(IR2Value);


  if (myTemp < 60) {
    digitalWrite(Relay, HIGH);
  }

  if (myTemp >= 60) {
    digitalWrite(Relay, LOW);
  }

  ultraSonicFunction();
  Serial.println(ultraSonic1);
  Serial.println(ultraSonic2);
  Serial.println(ultraSonic3);

  ultraSonic1 = 5;
  ultraSonic2 = 5;
  ultraSonic3 = 5;


  sensors.requestTemperatures();
  Serial.print("Temperature: ");
  myTemp = sensors.getTempCByIndex(0);
  Serial.println(myTemp);


  // ESP32 serial part is here:

  if (Serial.available() ) {
    char c = Serial.read();
    Serial.println( c);

    if (c == '0') {

    } else if (c == '1') {
      normal();
    } else if (c == '2') {
      noSugar();
    } else if (c == '3') {
      sugarPlus();
    } else if (c == '4') {
      dark();
    }

  }

  // Keypad part is here:

  if (key >= '0' && key <= '9') {
    inputString += key;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(inputString);
  }
  else if (key == '#') {
    if (inputString == "0") {
      normal();
    } else if (inputString == "1") {
      noSugar();
    } else if (inputString == "2") {
      sugarPlus();
    } else if (inputString == "3") {
      dark();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose from 0->3");
      lcd.setCursor(0, 1);
      lcd.print("Then press #");
      inputString = "";
      key = '/';
      delay(2000);
      lcd_screen_init();
    }
  }

}
