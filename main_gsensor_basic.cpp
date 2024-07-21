/**********************************************************************
 file: main_gsensor_basic.cpp - basic test of ADXL345 accelerometer

 Copyright (C) 2018  p. chu

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **********************************************************************/

/*********************************************************************
 * 4-bit SPI interface:
 *   - spi mode 4 (cpol=1, cpha=1)
 *   - write transfer format:
 *       - mosi: 0maaaaaa dddddddd ...
 *       - 0 for write; m (0/1): 1-byte/multi-byte data;
 *       - aaaaaa: 6-bit reg addr; ddddddd: 8-bit data
 *   - read transfer format
 *       - mosi: 1maaaaaa xxxxxxxx ...
 *       - miso: xxxxxxxx dddddddd ...
 *       - 1 for read ; m (0/1): 1-byte/multi-byte data;
 *       - aaaaaa: 6-bit reg addr; ddddddd: 8-bit data
 **********************************************************************
 * Initialization
 *   - device power on in standby mode
 *   - must set bit 3 of reg 0x2d to activate measurement mode
 *   - default 10-bit resolution in +/- 2g
 * Device id
 *   - read reg 0x00: should return 0xe5 (1110_0101)
 **********************************************************************/

// #define _DEBUG
#include "de10_baseline.h"

// write a single byte
void adx345_write(uint8_t addr, uint8_t tx_data) {
   uint8_t addr_byte = addr & 0x3f;
   spi_acc.assert_ss(0);         // activate
   spi_acc.transfer(addr_byte);  // transfer addr byte
   spi_acc.transfer(tx_data);    // transfer data byte
   spi_acc.deassert_ss(0);       // deactivate
}

// read a single byte
uint8_t adx345_read(uint8_t addr) {
   uint8_t addr_byte = 0x80 | (addr & 0x3f);
   spi_acc.assert_ss(0);         // activate
   spi_acc.transfer(addr_byte);  // transfer addr byte
   uint8_t rx_data = spi_acc.transfer(0x00); // transfer a dummy byte
   spi_acc.deassert_ss(0);
   return (rx_data);
}

int main() {
   const uint8_t ID_REG = 0x00;
   const uint8_t POWER_CTRL_REG = 0x2d;
   const uint8_t DATAZ0_REG = 0x36;

   // set up SPI
   spi_acc.set_freq(400000);  // 400K Hz
   spi_acc.set_mode(1, 1);
   // activate measurement mode
   adx345_write(POWER_CTRL_REG, 0x08);
   while (1) {
      // id check: should be 0xe5 (1110_0101)
      uint32_t id = adx345_read(ID_REG);
      led.write(id);
      // read z-axis acceleration (g)
      uint8_t z_low = adx345_read(DATAZ0_REG);
      uint8_t z_high = adx345_read(DATAZ0_REG + 1);
      // type casting: 1st signed and then extension
      int z = (int) (int8_t) z_high;
      // pack
      z = (z << 8) | z_low;
      // display g
      int g = map(z, -512, 512, -200, 200); // 200 for 2.00g
      sseg.i2sseg(g, 10, 4, 0, 1, 1);
      sseg.set_dp(0x04);
      // 5 Hz sampling rate
      sleep_ms(200);
   } //while
} //main

