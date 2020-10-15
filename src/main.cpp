#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "LoRaMessage.h"
#include "BME280.h"
#include "USBSerial.h"
#include "Battery.h"
#include "keyFactory.h"
#include "chrono"
#include "settings.h"

using namespace SimpleLoRaWAN;

int main(void)
{
  DigitalOut led(D13);
  led = 1;
  ThisThread::sleep_for(10ms);
  led = 0;

  Battery battery;
  
  BME280 bme280(PB_7, PB_6);
  bme280.getTemperature();
  bme280.getHumidity();
  bme280.getPressure();

  KeyFactory keyFactory(appEui);
  
  {
    USBSerial serial(false);
    serial.connect();

    for(int i = 0; i < 10; i++) {

      if(serial.ready()) {
        serial.printf("\r\n*** Starting LoRaWAN Micro Weather Station ***\r\n");
        keyFactory.printKeys(&serial);
        break;
      }

      led = 1;
      ThisThread::sleep_for(10ms);
      led = 0;
      ThisThread::sleep_for(990ms);
    }
  } // destroy USBSerial object

  SimpleLoRaWAN::LoRaWANKeys keys = keyFactory.getKeys();
  SimpleLoRaWAN::Pinmapping pins = { PA_7, PA_6, PA_5, PB_0, PB_1, PA_8, PA_9 };
  Node node(keys, pins);
  
  while(true) {

    double voltage = battery.getVoltage();
    double temperature = bme280.getTemperature();  // value in °C
    double humidity = bme280.getHumidity();        // value in %
    double pressure = bme280.getPressure();        // value in hPa

    LoRaMessage message;
    message.addUint8( ((int) (voltage * 150 / 3) )); // 0V - 3V --> 0 - 150 (0.02V resolution)
    message.addUint16(temperature * 100);
    message.addUint16(humidity * 100);
    message.addUint16(pressure * 10);

    node.send(message.getMessage(), message.getLength());
    ThisThread::sleep_for(300s);
  }
}