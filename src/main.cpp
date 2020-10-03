#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "LoRaMessage.h"
#include "settings.h"
#include "BME280.h"

using namespace SimpleLoRaWAN;

AnalogIn vrefint(ADC_VREF);

DigitalOut led(D13);

Node node(keys, pins);

int main(void)
{
  // led = 1;

  uint16_t vref_cal;
  vref_cal = *(__IO uint16_t *)((uint32_t)0x1fff75aa);
  
  BME280 bme280(PB_7, PB_6);
  bme280.getTemperature();
  bme280.getHumidity();
  bme280.getPressure();

  led = 1;
  ThisThread::sleep_for(100ms);
  led = 0;

  // printf("\r\n*** Starting LoRaWAN Shield Example ***\r\n");
  
  while(true) {

    LoRaMessage message;

    // measure battery voltage
    double voltage = (3.f * (double)vref_cal / 4096) / vrefint.read();
    message.addUint8( ((int) (voltage * 150 / 3) )); // 0V - 3V --> 0 - 150 (0.02V resolution)

    // measure temperature, pressure and humidity
    // tph_sensor.awake();
    double temperature = bme280.getTemperature();  // value in °C
    double humidity = bme280.getHumidity();        // value in %
    double pressure = bme280.getPressure();        // value in hPa
    // tph_sensor.sleep();
    message.addUint16(temperature * 100);
    message.addUint16(humidity * 100);
    message.addUint16(pressure * 10);

    node.send(message.getMessage(), message.getLength());
    ThisThread::sleep_for(300s);
    // for(int i = 0; i < 30; i++){
    //   led = 1;
    //   ThisThread::sleep_for(10ms);
    //   led = 0;
    //   ThisThread::sleep_for(9990ms);
    // }
  }
}