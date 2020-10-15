#pragma once

#include "mbed.h"

class Battery {
  public:
    Battery();
    double getVoltage();
  private:
    // AnalogIn vrefint;
    // uint16_t vref_cal;
    // const uint32_t VREF_REGISTER = 0x1FFF75AA;
};