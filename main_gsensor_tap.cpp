/**********************************************************************
 file: main_gsensor_tap.cpp - "tap" test of ADXL345 accelerometer

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
   const uint8_t POWER_CTRL_REG = 0x2d;
   const uint8_t TAP_REG = 0x30;
   uint32_t led_pattern = 0x1f;

   // set up SPI
   spi_acc.set_freq(400000);  // 400K Hz
   spi_acc.set_mode(1, 1);
   // activate measurement mode
   adx345_write(POWER_CTRL_REG, 0x08);
   // set up tap functionality
   adx345_write(0x1d, 0x20); // acc threshold = 3g
   adx345_write(0x21, 0x10); // duration 0.01s
   adx345_write(0x22, 0x50); // latency 0.1s
   adx345_write(0x23, 0xf0); // window 0.3s
   adx345_write(0x2a, 0x07); // all axis
   adx345_write(0x2e, 0xff); // activate functionality
   // turn on led
   led.write(led_pattern);
   while (1) {
      // read tap status register
      int tap = adx345_read(TAP_REG) & 0x60;
      // toggle led pattern with "single-tapping"
      if (tap == 0x40){
         led_pattern = ~led_pattern;
         led.write(led_pattern);
      }   
     // turn off with "double-tapping"
     if (tap == 0x60)
         led.write(0);
      sleep_ms(50);
   } //while
} //main

