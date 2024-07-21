#ifndef ADXL345_H
#define ADXL345_H


class ADXL345
{
    public:
        int init();
        void read_acc(float *x, float *y, float *z);

    protected:

    private:
        uint8_t read_byte(uint8_t reg);
        uint8_t write_byte(uint8_t reg, uint8_t data_byte);
};

#endif // ADXL345_H
