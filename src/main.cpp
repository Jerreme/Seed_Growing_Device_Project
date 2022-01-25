//Libraries & Dependencies
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>


//Initialization
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
dht DHT;
String saturation;

//Declaration
int count = 0, 
    battery = 0;
bool analytic = false, 
    r_charging = false,
    r_battery = false;

//Pins
#define servo_pin 3
#define relayPump_pin 4
#define relayLED_pin 5

#define ir1_pin 10
#define ir2_pin 9

#define moist1_pin 13
#define moist2_pin 12

#define ldr_pin A0
#define volt_pin A1
#define dht_pin A2
#define buzzer_pin A3

void setup() {
  // put your setup code here, to run once:
  pinMode(servo_pin, OUTPUT);
  pinMode(relayPump_pin, OUTPUT);
  pinMode(relayLED_pin, OUTPUT);

  pinMode(ir1_pin, INPUT);
  pinMode(ir2_pin, INPUT);

  pinMode(moist1_pin, INPUT);
  pinMode(moist2_pin, INPUT);

  pinMode(ldr_pin, INPUT);
  pinMode(volt_pin, INPUT);
  pinMode(dht_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  myServo.attach(servo_pin);
  myServo.write(0);

  lcd.init();                      // initialize the lcd 
  // lcd.backlight();
  lcd.print("Starting...");
  
  
  Serial.begin(9600);
  delay(5000);

  // Logon Sound
  tone(buzzer_pin,700);
  delay(120);
  tone(buzzer_pin,900);
  delay(200);
  noTone(buzzer_pin);

  lcd.clear();
  lcd.backlight();
}
//----------------------------------------------------------------------------//



void display(String toDisplay) {
  // lcd.clear();
  lcd.print(toDisplay);
}

double readVoltage() {
  int read = analogRead(volt_pin);
  double voltage = map(read, 0, 1023, 0, 2500) + 4;
  voltage /= 100;
  
  return voltage;
}

bool readMoisture(int pin) {
  bool read = digitalRead(pin);
  return read;
}

bool readIR(int pin) {
  bool val = digitalRead(pin);
  return !val;
}

int readSun() {
  int val = analogRead(ldr_pin);
  
  Serial.println(val);
  return val;
}

int readDHT11(String read) {
  DHT.read11(dht_pin);
  if(read.equals("T")) {
    return int(DHT.temperature);
  }

  if(read.equals("H")) {
    return int(DHT.humidity);
  }
}

void buzzer(byte state) {
  switch(state) {
    case 0:                       // 0 = Low Battery
        for(int i=0; i<3; i++) {
          tone(buzzer_pin, 400, 300);
          delay(500);
        }
      break;
    case 1:                       // 1 = Fully Charged 
      tone(buzzer_pin, 600);
      delay(60);
      tone(buzzer_pin, 800);
      delay(30);
      tone(buzzer_pin, 700);
      delay(80);
      tone(buzzer_pin, 1500);
      delay(60);
      tone(buzzer_pin, 2200);
      delay(70);
      noTone(buzzer_pin);
      break;
    case 2:                       // 2 = Pumping
      tone(buzzer_pin, 780, 750);
      delay(1000);
      break;
  }
}

int sun_val = 0;
String convert_sun(int val) {
  String result;
  if(val > 1000 && val <= 1023) {
    result = "Night ";
  }else if (val > 600 &&  val <= 1000) {
    result = "Dim   ";
  }else if (val > 170 &&  val <= 600) {
    result = "Clear ";
  }else if (val > 0 &&  val <= 170) {
    result = "Sunny ";
  }else {
    result = "Error ";
  }

  sun_val = map(val, 0, 1023, 100, 0);
  return result;
}

String convert_temp(int val) {
  String result;
  if(val >= 40) {
    result = "Hot   ";
  }else if (val >= 36 && val <= 39) {
    result = "Warm  ";
  }else if (val >= 32 && val <= 35) {
    result = "Mid   ";
  }else if (val >= 26 && val <= 31) {
    result = "Low   "; 
  }else if (val < 26) {
    result = "Cold  ";
  }else {
    result = "Error ";
  }

  return result;
}

String convert_humidity(int val) {
  String result;
  if(val >= 70) {
    result = "High  ";
  }else if (val >= 60 && val < 70) {
    result = "Mid   ";
  }else if (val >= 30 && val < 60) {
    result = "Good  ";
  }else if (val >= 25 && val < 30) {
    result = "Low   "; 
  }else if (val < 25) {
    result = "Dry   ";
  }else {
    result = "Error ";
  }

  return result;
}

double convert_voltage(double val) {
  double result = 0;

  val -= 3.4;
  result = val * 100;
  val = result / 80;
  result = val *100;
  return result;
}


void pumpWater(String plant) {
  int pos = myServo.read();

  if(plant.equals("plant1")) {
    lcd.clear();
    lcd.print("Watering Plant 1");
    buzzer(2);

    for(int i=pos; i<170; i++) {
      myServo.write(i);
      delay(10);
    }
    delay(2000);

    digitalWrite(relayPump_pin, HIGH);
    delay(500);
    // delay(500);
    // for(int i=0; i<2; i++) {
    //   for(int i=pos; i>145; i--) {
    //     myServo.write(i);
    //     delay(15);
    //   }

    //   for(int i=pos; i<170; i++) {
    //     myServo.write(i);
    //     delay(15);
    //   }
    // }
    

    delay(2000);
    digitalWrite(relayPump_pin, LOW);
    delay(1000);
    lcd.clear();
    lcd.print("Plant 1 Done!");
    delay(2000);

  }else if(plant.equals("plant2")) {
    lcd.clear();
    lcd.print("Watering Plant 2");
    buzzer(2);
    
    for(int i=pos; i>0; i--) {
      myServo.write(i);
      delay(10);
    }
    delay(2000);

    digitalWrite(relayPump_pin, HIGH);
    delay(500);
    // for(int i=0; i<2; i++) {
    //   for(int i=pos; i<35; i++) {
    //     myServo.write(i);
    //     delay(15);
    //   }

    //   for(int i=pos; i>0; i--) {
    //     myServo.write(i);
    //     delay(15);
    //   }
    // }

    delay(2000);
    digitalWrite(relayPump_pin, LOW);
    delay(1000);
    lcd.clear();
    lcd.print("Plant 2 Done!");
    delay(2000);
  }
}

void alarms(bool type) {
  if(!r_battery) {
    r_battery = true;
    lcd.clear();
    lcd.setCursor(0,0);
    display("Low Battery");
    lcd.setCursor(3,1);
    display("Please Charge");
    if(type) {
      buzzer(0);
    }
    delay(1000);
  }
}


void loop() {
  // // put your main code here, to run repeatedly:
  if(!analytic) { 
    lcd.setCursor(0,0);
    saturation = convert_sun(readSun());
    display("S: " + saturation);

    lcd.setCursor(9,0);
    display("T: " + convert_temp(readDHT11("T")));

    lcd.setCursor(0,1);
    display("H: " + convert_humidity(readDHT11("H")));

    lcd.setCursor(9,1);
    battery = int(convert_voltage(readVoltage()));
    display("V: " + String(battery) + "% ");
    
    }else {
      lcd.setCursor(0,0);
      saturation = convert_sun(readSun());
      display("S: " + String(sun_val) + "%   ");

      lcd.setCursor(9,0);
      display("T: " + String(readDHT11("T")) + "C ");
      
      lcd.setCursor(0,1);
      display("H: " + String(readDHT11("H")) + "%   ");

      lcd.setCursor(9,1);
      display("V: " + String(readVoltage()));
    }
  
  count++;

  // Turn Lights at Night
  // if(readSun() > 1000 && int(convert_voltage(readVoltage())) > 60) {
  //   digitalWrite(relayLED_pin, HIGH);
  // }else {
  //   digitalWrite(relayLED_pin, LOW);    
  // }

  // Water the plant
  if(readMoisture(moist1_pin)) {
    if(!readIR(ir1_pin)) {
      pumpWater("plant1");
    }
  }else if(readMoisture(moist2_pin)) {
    if(!readIR(ir2_pin)) {
      pumpWater("plant2");
    }
  }

  // Alarms
  if(battery > 15 && battery < 20) {
    alarms(0);
  }else if(battery <= 15 && !r_battery) {
    alarms(1);
  }else if(battery == 100 && !r_battery) {
    // buzzer(1);
  }else {
    r_battery = false;
  }

  // Display Notifications
  if(saturation.equals("Sunny")) {
    if(!r_charging) {
      r_charging = true;
      lcd.clear();
      display("Charging...");
      delay(3000);
    }
  }else {
    r_charging = false;
  }


  if(count >= 20 && analytic == false) {
    count = 0;
    analytic = true;
    lcd.clear();
  }else if(count >= 20 && analytic == true) {
    count = 0;
    analytic = false;
    lcd.clear();
  }

  delay(200);
}
