/* Seed Growing Device Project
 * - an smart irrigation system that grows seed and or small plants.
 */ 

#include <Arduino.h>
#include <Sensors.h>
#include <Functions.h>

//Define Pins
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

//Classes
// -Object for our Sensor Library-
Sensors S;  
// -Object for our Function Library-
Functions F = Functions(servo_pin, buzzer_pin, relayPump_pin, relayLED_pin);

//Variabes
int count = 0;
int battery = 0;

int sun;
int temp;
int humid;
double volt;

bool analytic = false;
bool trig = false;

//------------------------------------------//

void setup() {

  // -Define pinouts-
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
  
  // Boot for 5 Seconds then beep
  F.START();  
  // Serial.begin(9600);
}

void loop() {

  // -Read Sensors-
  sun = S.readSun(ldr_pin);
  temp = S.readTemp(dht_pin);
  humid = S.readHumidity(dht_pin);
  volt = S.readVoltage(volt_pin);
  battery = F.getBattPercentage();

  // -Display Data-
  if(!analytic) { 
    //Readable Display
    F.display("S: " + F.convert_sun(sun), 0, 0);
    F.display("T: " + F.convert_temp(temp), 9, 0);
    F.display("H: " + F.convert_humidity(humid), 0, 1);
    F.display("V: " + F.convert_voltage(volt) + "% ", 9, 1);
    
  }else {
    //Analytic Display
    F.display("S: " + String(F.getSunVal(sun)) + "%   ", 0, 0);
    F.display("T: " + String(temp) + "C ", 9, 0);
    F.display("H: " + String(humid) + "%   ", 0, 1);
    F.display("V: " + String(volt), 9, 1);
  } 

  // -Turn Lights at Night-
  // if(sun > 1000 && battery > 80) {    // Turn Led Lights if Dark & has >70% battery
  //   digitalWrite(relayLED_pin, HIGH);
  // }else if(battery < 30 || sun < 1000) {
  //   digitalWrite(relayLED_pin, LOW);  // Turn back off during daytime
  // }

  // -Low Battery Alarm-
  if(battery == 30) {     // Alarm if battery dip to 30%
    F.buzzer("Low Battery");
  }

  // -Solar Charging-
  if(F.convert_sun(sun).equals("Sunny ")) { // Charge with Enough sun 
    if(!trig) {
      trig = true;
      F.display("Charging...", 0, 0);
      delay(3000);
    }
  }else{
    trig = false;
  }

  
  // -Water the plant-
  if(S.readMoisture(moist1_pin)) {        // True if the soil is Dry
    if(S.readIR(ir1_pin)) {              // True if pot is present
      F.pumpWater("Plant 1");
    }
  }else if(S.readMoisture(moist2_pin)) {  // True if the soil is Dry
    if(S.readIR(ir2_pin)) {              // True if pot is present
      F.pumpWater("Plant 2");
    }
  }

  // -Swap Display-
  if(count >= 20 ) {  // Swap Displays every 5 seconds(250ms*20)
    if(analytic) 
      analytic = false;
    else 
      analytic = true;
    
    count = 0;        // Reset count
    F.clearDisplay(); 
  }

  count++; 
  delay(250);
}
