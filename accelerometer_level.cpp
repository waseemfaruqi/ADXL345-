#include "ADXL345.h"
#include "SpiCore.h"
#include "de10_baseline.h"

int main()
{
    ADXL345 accelerometer;
    float x, y, z;
    while(accelerometer.init())
    {
        accelerometer.read_acc(&x, &y, &z);
        sleep_ms(200);
        //Check level and switch ON appropriate LED
        if(x > -2.00 && x < -1.60)
            led.write(1, 10);
        else if(x > -1.60 && x < -1.20)
            led.write(1, 9);
        else if(x > -1.20 && x < -0.80)
            led.write(1, 8);
        else if(x > -0.80 && x < -0.40)
            led.write(1, 7);
        else if(x > -0.40 && x < 0.0)
            led.write(1, 6);
        else if(x > 0.0 && x < 0.40)
            led.write(1, 5);
        else if(x > 0.40 && x < 0.80)
            led.write(1, 4);
        else if(x > 0.80 && x < 1.20)
            led.write(1, 3);
        else if(x > 1.20 && x < 1.60)
            led.write(1, 2);
        else if(x > 1.60 && x < 2.0)
            led.write(1, 1);
    }
}
