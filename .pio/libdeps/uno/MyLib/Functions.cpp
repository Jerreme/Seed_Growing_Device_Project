#include "Arduino.h"
#include "Functions.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//---------------------------------------------------------------------------//
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;



Functions::Functions(uint8_t servo, uint8_t buzz, uint8_t pump, uint8_t led) {
  servoPin = servo;
  buzzPin = buzz;
  pumpPin = pump;
  ledPin = led;  
}

void Functions::START() {
  myServo.attach(servoPin);

  // int pos = myServo.read();
  // if(pos < 180 && pos > 90)       // Rotate to plant 1
  //   myServo.write(180);
  // else if(pos > 0 && pos < 90)  // Rotate to plant 2
  //   myServo.write(0);

  myServo.write(0);
  delay(100);

  lcd.init();
  display("SMART IRRIGATION", 0, 0);
  display("     SYSTEM     ", 0, 1);
  delay(5000);

  // Logon Sound
  tone(buzzPin,700);
  delay(120);
  tone(buzzPin,900);
  delay(200);
  noTone(buzzPin);

  lcd.clear();
  lcd.backlight();
}

void Functions::display(String toDisplay, uint8_t col, uint8_t rows) {
  if(toDisplay.equals("Charging..."))
    clearDisplay();

  lcd.setCursor(col,rows);
  lcd.print(toDisplay);
}

void Functions::clearDisplay() {
  lcd.clear();
}

void Functions::buzzer(String str) {
    str.toUpperCase();
    char state = str.charAt(0);
    
    switch(state) {
        case 'L':   // Low Battery
            lcd.clear();
            display("Low Battery", 0, 0);
            display("Please Charge!", 3, 1);

            for(int i=0; i<3; i++) {
              tone(buzzPin, 400, 300);
              delay(500);
            }
        break;
        case 'F':   // Fully Charged 
            tone(buzzPin, 600);
            delay(60);
            tone(buzzPin, 800);
            delay(30);
            tone(buzzPin, 700);
            delay(80);
            tone(buzzPin, 1500);
            delay(60);
            tone(buzzPin, 2200);
            delay(70);
            noTone(buzzPin);
            break;
        case 'W':   // Watering Plant
            tone(buzzPin, 780, 750);
            delay(1000);
            break;
        default:
          break;
    }
}

void Functions::pumpWater(String plant) {
  short pos = myServo.read();

  lcd.clear();
  lcd.print("Watering "+ plant);
  buzzer("watering Plant");

  if(plant.equals("Plant 1")) { 
    for(pos; pos<185; pos++) {
      myServo.write(pos);
      delay(10);
    }
  }else if(plant.equals("Plant 2")) {
    for(pos; pos>0; pos--) {
      myServo.write(pos);
      delay(10);
    }
  }

  delay(2000);
  digitalWrite(pumpPin, HIGH);
  delay(2000);
  digitalWrite(pumpPin, LOW);
  delay(500);

  lcd.clear();
  lcd.print(plant + " Done!");
  delay(2000);
}


String Functions::convert_sun(int sun_val) {
  if(sun_val > 1000 && sun_val <= 1023) {
    result = "Night ";
  }else if (sun_val > 600 &&  sun_val <= 1000) {
    result = "Dim   ";
  }else if (sun_val > 170 &&  sun_val <= 600) {
    result = "Clear ";
  }else if (sun_val > 0 &&  sun_val <= 170) {
    result = "Sunny ";
  }else {
    result = "Error ";
  }
  return result;
}

int Functions::getSunVal(int sun_val) {
  return map(sun_val, 0, 1023, 100, 0);
}

String Functions::convert_temp(int temp_val) {
  if(temp_val >= 40) {
    result = "Hot   ";
  }else if (temp_val >= 36 && temp_val <= 39) {
    result = "Warm  ";
  }else if (temp_val >= 32 && temp_val <= 35) {
    result = "Mid   ";
  }else if (temp_val >= 26 && temp_val <= 31) {
    result = "Low   "; 
  }else if (temp_val < 26) {
    result = "Cold  ";
  }else {
    result = "Error ";
  }

  return result;
}

String Functions::convert_humidity(int humid_val) {
  if(humid_val >= 70) {
    result = "High  ";
  }else if (humid_val >= 60 && humid_val < 70) {
    result = "Mid   ";
  }else if (humid_val >= 30 && humid_val < 60) {
    result = "Best  ";
  }else if (humid_val >= 25 && humid_val < 30) {
    result = "Low   "; 
  }else if (humid_val < 25) {
    result = "Dry   ";
  }else {
    result = "Error ";
  }

  return result;
}

String Functions::convert_voltage(double volt_val) {
  volt_val -= 3.4;
  percentage = volt_val * 100;
  volt_val = percentage / 80;
  percentage = volt_val *100;
  
  return String(int(percentage));
}

int Functions::getBattPercentage() {
  return percentage;
}




