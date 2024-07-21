#ifndef SPICORE_H_INCLUDED
#define SPICORE_H_INCLUDED

class SpiCore {
public:
    // register map
    enum {
    RD_DATA_REG = 0, // 8-bit read data reg
    SS_REG = 1, // status register
    WRITE_DATA_REG = 2, // write data register
    CTRL_REG = 3 }; // control reg
    // (ss/cpha/cpol/dvs)
    // Field masks
    enum {
    READY_FIELD = 0x00000100, // ready bit
    RX_DATA_FIELD = 0x000000ff }; // read data
    // constructor
    SpiCore(uint32_t core_base_addr);
    // spi core is ready for transfer
    int ready();
    // set spi bus clock frequency
    void set_freq(int freq);
    // set spi mode (clock polarity/phase)
    void set_mode(int icpol, int icpha);
    // write ss_n register
    void write_ss_n(uint32_t data);
    // write ss_n bit specified by bit_pos
    void write_ss_n(int bit_value, int bit_pos);
    // assert slave select (ss_n) of device n
    void assert_ss(int n);
    // deassert slave select (ss_n) of device n
    void deassert_ss(int n);
    // shift out write data and shift in read data
    uint8_t transfer(uint8_t wr_data);
private:
    uint32_t base_addr;
    uint32_t ss_n_data;
    uint16_t dvsr;
    int cpol;
    int cpha;
};

#endif // SPICORE_H_INCLUDED
