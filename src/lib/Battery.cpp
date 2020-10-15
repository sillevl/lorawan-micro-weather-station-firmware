#include "Battery.h"

Battery::Battery() {
  // vref_cal = *(__IO uint16_t *)((uint32_t)VREF_REGISTER);
}

double Battery::getVoltage() {
  AnalogIn vrefint(ADC_VREF);
  uint16_t vref_cal;
  vref_cal = *(__IO uint16_t *)((uint32_t)0x1FFF75AA);
  double voltage = (3.f * (double)vref_cal / 4096) / vrefint.read();
  return voltage;
}