#ifndef Functions_h
#define Functions_h

#include "Arduino.h"

class Functions {
    public:
        Functions(uint8_t servo, uint8_t buzz, uint8_t pump, uint8_t led) ;

        void START();
        void display(String toDisplay,  uint8_t col, uint8_t rows);
        void buzzer(String str);
        void pumpWater(String plant);
        void clearDisplay();

        String convert_sun(int sun_val);
        String convert_temp(int temp_val);
        String convert_humidity(int humid_val);
        String convert_voltage(double val);
        
        int getBattPercentage();
        int getSunVal(int sun_val);
        
    private:
        uint8_t servoPin;
        uint8_t buzzPin;
        uint8_t pumpPin;
        uint8_t ledPin;

        double percentage;
        String result;
};

#endif