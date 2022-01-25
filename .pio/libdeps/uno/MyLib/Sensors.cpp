#include "Arduino.h"
#include "Sensors.h"
#include "dht.h"

//------------------------------------------------------//
dht DHT;

double Sensors::readVoltage(uint8_t pin) {
  voltage = map(analogRead(pin), 0, 1023, 0, 2500) + 4;
  voltage /= 100;
  return voltage;
}

bool Sensors::readMoisture(uint8_t pin) {
  return digitalRead(pin);
}

bool Sensors::readIR(uint8_t pin) {
  return digitalRead(pin);
}

int Sensors::readSun(uint8_t pin) {  
  return analogRead(pin);;
}

int Sensors::readTemp(uint8_t pin) {
    DHT.read11(pin);
    return int(DHT.temperature);
}

int Sensors::readHumidity(uint8_t pin) {
    DHT.read11(pin);
    return int(DHT.humidity);
}

