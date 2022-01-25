#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"

class Sensors {
    public:
        double readVoltage(uint8_t pin);
        bool readMoisture(uint8_t pin);
        bool readIR(uint8_t pin);
        
        int readSun(uint8_t pin);
        int readTemp(uint8_t pin);
        int readHumidity(uint8_t pin);

    private:
        double voltage;  
};

#endif