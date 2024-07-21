#include "ADXL345.h"
#include "de10_baseline.h"
#include "SpiCore.h"

int ADXL345::init()
{
    //Set SPI Clock
    acc_spi.set_freq(400000);
    //Set SPI Mode
    acc_spi.set_mode(1, 1);
    //Activate Measurement Mode
    write_byte(0x2d, 0x08);
    //Configure and activate tapping activity
    write_byte(0x1d, 0x20);
    write_byte(0x21, 0x10);
    write_byte(0x22, 0x50);
    write_byte(0x23, 0xf0);
    write_byte(0x2a, 0x07);
    write_byte(0x2e, 0xff);
    //Return 1 if device Id matches
    if(read_byte(0x00) == 0xe5)
        return 1;
    else
        return 0;
}

void ADXL345::read_acc(float *x, float *y, float *z)
{
    //Read lower byte and upper byte from the sensor X, y and Z registers
    uint8_t xL = read_byte(0x32);
    uint8_t xH = read_byte(0x33);
    uint8_t yL = read_byte(0x34);
    uint8_t yH = read_byte(0x35);
    uint8_t zL = read_byte(0x36);
    uint8_t zH = read_byte(0x37);
    //Converting from 2's Complement and combining into one variable
    int xHL = (int) (int8_t) xH;
    xHL = (xHL << 8) | xL;
    *x = (float) xHL;
    *x = (float) (4.00 * (*x + 512)/(1024))-2.00;
    //Converting from 2's Complement and combining into one variable
    int yHL = (int) (int8_t) yH;
    yHL = (yHL << 8) | yL;
    *y = (float) yHL;
    *y = (float) (4.00 * (*y + 512)/(1024))-2.00;
    //Converting from 2's Complement and combining into one variable
    int zHL = (int) (int8_t) zH;
    zHL = (zHL << 8) | zL;
    *z = (float) zHL;
    *z = (float) (4.00 * (*z + 512)/(1024))-2.00;
}

uint8_t ADXL345::read_byte(uint8_t reg)
{
    acc_spi.assert_ss(0);                       // activate
    acc_spi.transfer( 0x80 | (reg & 0x3f) );    // transfer addr byte
    uint8_t data = acc_spi.transfer(0x00);      // transfer a dummy byte
    acc_spi.deassert_ss(0);
    return (data);
}

uint8_t ADXL345::write_byte(uint8_t reg, uint8_t data_byte)
{
    acc_spi.assert_ss(0);           // activate
    acc_spi.transfer(reg & 0x3f);   // transfer addr byte
    acc_spi.transfer(data_byte);    // transfer data byte
    acc_spi.deassert_ss(0);         // deactivate
}
