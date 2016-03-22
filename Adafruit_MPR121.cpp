/***************************************************
  This is a library for the MPR121 I2C 12-chan Capacitive Sensor

  Designed specifically to work with the MPR121 sensor from Adafruit
  ----> https://www.adafruit.com/products/1982

  These sensors use I2C to communicate, 2+ pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_MPR121.h"

Adafruit_MPR121::Adafruit_MPR121() {
}

boolean Adafruit_MPR121::begin(uint8_t i2caddr) {
  Wire.begin();
  _i2caddr = i2caddr;

  // soft reset
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  for (uint8_t i=0; i<0x7F; i++) {
  //  Serial.print("$"); Serial.print(i, HEX);
  //  Serial.print(": 0x"); Serial.println(readRegister8(i));
  }
  m_mode = DISABLED;
  writeRegister(MPR121_ECR, 0x0);

  uint8_t c = readRegister8(MPR121_CONFIG2);

  if (c != 0x24) return false;

  setThresholds(12, 6);
  setChargeCurrent(16);

  writeRegister(MPR121_MHDR, 0x01);
  writeRegister(MPR121_NHDR, 0x01);
  writeRegister(MPR121_NCLR, 0x0E);
  writeRegister(MPR121_FDLR, 0x00);

  writeRegister(MPR121_MHDF, 0x01);
  writeRegister(MPR121_NHDF, 0x05);
  writeRegister(MPR121_NCLF, 0x01);
  writeRegister(MPR121_FDLF, 0x00);

  writeRegister(MPR121_NHDT, 0x00);
  writeRegister(MPR121_NCLT, 0x00);
  writeRegister(MPR121_FDLT, 0x00);

  writeRegister(MPR121_DEBOUNCE, 0);

  // proxi registers
  writeRegister(MPR121_PROXI_MHDR, 0xFF);
  writeRegister(MPR121_PROXI_NHDR, 0xFF);
  writeRegister(MPR121_PROXI_NCLR, 0x00);
  writeRegister(MPR121_PROXI_FDLR, 0x00);
  writeRegister(MPR121_PROXI_MHDF, 0x01);
  writeRegister(MPR121_PROXI_NHDF, 0x01);
  writeRegister(MPR121_PROXI_NCLF, 0xFF);
  writeRegister(MPR121_PROXI_FDLF, 0xFF);
  writeRegister(MPR121_PROXI_NHDT, 0x00);
  writeRegister(MPR121_PROXI_NCLT, 0x00);
  writeRegister(MPR121_PROXI_FDLT, 0x00);

  set_mode(ENABLED);

//  writeRegister(MPR121_AUTOCONFIG0, 0x8F);
//  writeRegister(MPR121_UPLIMIT, 150);
//  writeRegister(MPR121_TARGETLIMIT, 100); // should be ~400 (100 shifted)
//  writeRegister(MPR121_LOWLIMIT, 50);

  return true;
}

Adafruit_MPR121::Mode Adafruit_MPR121::mode() const
{
    return m_mode;
}

void Adafruit_MPR121::set_mode(Adafruit_MPR121::Mode m)
{
    // if(m_mode != m)
    {
        writeRegister(MPR121_ECR, m);
        m_mode = m;
    }
}

void Adafruit_MPR121::setThresholds(uint8_t touch, uint8_t release) {
  scoped_disable sd(this);

  for (uint8_t i = 0; i < 13; i++) {
    writeRegister(MPR121_TOUCHTH_0 + 2*i, touch);
    writeRegister(MPR121_RELEASETH_0 + 2*i, release);
  }
}

void Adafruit_MPR121::setChargeCurrent(uint8_t mc){
  if(mc > 63){ mc = 63; }
  scoped_disable sd(this);
  writeRegister(MPR121_CONFIG1, mc);
  writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period
}

void setChannelChargeCurrent(uint8_t ch, uint8_t mc){
  if(mc > 63){ mc = 63; }
  // writeRegister(MPR121_CONFIG1, mc);
}

uint16_t Adafruit_MPR121::filteredData(uint8_t t) {
  if (t > 12) return 0;
  return readRegister16(MPR121_FILTDATA_0L + t*2);
}

uint16_t  Adafruit_MPR121::baselineData(uint8_t t) {
  if (t > 12) return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

uint16_t  Adafruit_MPR121::touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x1FFF;
}

/*********************************************************************/


uint8_t Adafruit_MPR121::readRegister8(uint8_t reg) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(_i2caddr, 1) != 1);
    return ( Wire.read());
}

uint16_t Adafruit_MPR121::readRegister16(uint8_t reg) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    while (Wire.requestFrom(_i2caddr, 2) != 2);
    uint16_t v = Wire.read();
    v |=  ((uint16_t) Wire.read()) << 8;
    return v;
}

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void Adafruit_MPR121::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)(value));
    Wire.endTransmission();
}
