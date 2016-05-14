/***************************************************
  This is a library for the MPR121 12-Channel Capacitive Sensor

  Designed specifically to work with the MPR121 breakout from Adafruit
  ----> https://www.adafruit.com/products/1982

  These sensors use I2C to communicate, 2+ pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  04-2016 crocdialer@googlemail.com:
  added access to MPR121 proximity sensing.
  different configurations are available to provide a virtual 13th electrode
  for proximity sensing.
 ****************************************************/

#ifndef ADAFRUIT_MPR121_H
#define ADAFRUIT_MPR121_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>

// The default I2C address
#define MPR121_I2CADDR_DEFAULT 0x5A

#define MPR121_TOUCHSTATUS_L 0x00
#define MPR121_TOUCHSTATUS_H 0x01
#define MPR121_FILTDATA_0L  0x04
#define MPR121_FILTDATA_0H  0x05
#define MPR121_BASELINE_0   0x1E
#define MPR121_MHDR         0x2B
#define MPR121_NHDR         0x2C
#define MPR121_NCLR         0x2D
#define MPR121_FDLR         0x2E
#define MPR121_MHDF         0x2F
#define MPR121_NHDF         0x30
#define MPR121_NCLF         0x31
#define MPR121_FDLF         0x32
#define MPR121_NHDT         0x33
#define MPR121_NCLT         0x34
#define MPR121_FDLT         0x35

#define MPR121_PROXI_MHDR         0x36
#define MPR121_PROXI_NHDR         0x37
#define MPR121_PROXI_NCLR         0x38
#define MPR121_PROXI_FDLR         0x39
#define MPR121_PROXI_MHDF         0x3A
#define MPR121_PROXI_NHDF         0x3B
#define MPR121_PROXI_NCLF         0x3C
#define MPR121_PROXI_FDLF         0x3D
#define MPR121_PROXI_NHDT         0x3E
#define MPR121_PROXI_NCLT         0x3F
#define MPR121_PROXI_FDLT         0x40

#define MPR121_TOUCHTH_0    0x41
#define MPR121_RELEASETH_0    0x42
#define MPR121_DEBOUNCE 0x5B
#define MPR121_CONFIG1 0x5C
#define MPR121_CONFIG2 0x5D
#define MPR121_CHARGECURR_0 0x5F
#define MPR121_CHARGETIME_1 0x6C
#define MPR121_ECR 0x5E
#define MPR121_AUTOCONFIG0 0x7B
#define MPR121_AUTOCONFIG1 0x7C
#define MPR121_UPLIMIT   0x7D
#define MPR121_LOWLIMIT  0x7E
#define MPR121_TARGETLIMIT  0x7F

#define MPR121_GPIODIR  0x76
#define MPR121_GPIOEN  0x77
#define MPR121_GPIOSET  0x78
#define MPR121_GPIOCLR  0x79
#define MPR121_GPIOTOGGLE  0x7A

#define MPR121_SOFTRESET 0x80

//.. thru to 0x1C/0x1D
class Adafruit_MPR121 {
 public:

  enum Mode
  {
      DISABLED = 0x00,
      ENABLED = 0x8F,
      PROXI_01 = 0x9F,
      PROXI_04 = 0xAF,
      PROXI_ALL = 0xBF
  };

  // Hardware I2C
  Adafruit_MPR121(void);

  /*!
   * begin operation of a MPR121 device connected on i2c-bus with address i2caddr
   * Default address is 0x5A, if tied to 3.3V its 0x5B
   * If tied to SDA its 0x5C and if SCL then 0x5D
   */
  boolean begin(uint8_t i2caddr = MPR121_I2CADDR_DEFAULT);

  uint16_t filteredData(uint8_t t);
  uint16_t  baselineData(uint8_t t);
  uint16_t touched(void);

  void setThresholds(uint8_t touch, uint8_t release);

  //! global electrode charge current in uA, range [0 ... 63 uA]
  void setChargeCurrentAndTime(uint8_t the_current, uint8_t the_time = 1);

  //! channel specific measurement current in uA, range [0 ... 63 uA]
  void setChannelChargeCurrent(uint8_t ch, uint8_t mc);

  Mode mode() const;
  void setMode(Mode m);

 private:

   uint8_t readRegister8(uint8_t reg);
   uint16_t readRegister16(uint8_t reg);
   void writeRegister(uint8_t reg, uint8_t value);

   int8_t _i2caddr;
   Mode m_mode;
};

#endif // ADAFRUIT_MPR121_H
