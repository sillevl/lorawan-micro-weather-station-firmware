#include "mbed.h"
#include "Simple-LoRaWAN.h"
#include "LoRaMessage.h"
#include "settings.h"
#include "BME280.h"
#include "USBSerial.h"
#include "mbedtls/sha256.h" /* SHA-256 only */


using namespace SimpleLoRaWAN;

AnalogIn vrefint(ADC_VREF);

DigitalOut led(D13);

/**************************/
// static void print_hex(const char *title, const unsigned char buf[], size_t len)
// {
//     serial.printf("%s: ", title);

//     for (size_t i = 0; i < len; i++) {
//         if (buf[i] < 0xF) {
//             serial.printf("0%x", buf[i]);
//         } else {
//             serial.printf("%x", buf[i]);
//         }
//     }

//     serial.printf("\r\n");
// }
/*****************************/

int main(void)
{
  // led = 1;

  uint16_t vref_cal;
  vref_cal = *(__IO uint16_t *)((uint32_t)0x1FFF75AA);

  unsigned long *uid = (unsigned long *)0x1FFF7590;

  // serial.printf("\r\nUnique ID: %08X %08X %08X \r\n", uid[0], uid[1], uid[2]);

  /******************/
  static const char hello_str[] = "Hello, world!";
  static const unsigned char *hello_buffer = (const unsigned char *) hello_str;
  static const size_t hello_len = strlen(hello_str);

  unsigned char output1[32]; /* SHA-256 outputs 32 bytes */

  /* 0 here means use the full SHA-256, not the SHA-224 variant */
  mbedtls_sha256( (const unsigned char*) uid, 12, output1, 0);

  // print_hex("Method 1", output1, sizeof output1);
  // print_hex("UID", (const unsigned char*) uid, 12);

  // serial.printf("\r\ndone.");

  /************************/
  
  BME280 bme280(PB_7, PB_6);
  bme280.getTemperature();
  bme280.getHumidity();
  bme280.getPressure();

  led = 1;
  ThisThread::sleep_for(100ms);
  led = 0;


  SimpleLoRaWAN::LoRaWANKeys keys;
  std::copy(appEui, appEui + 8, keys.appEui);
  std::copy(output1 + 0, output1 + 8, keys.devEui);
  std::copy(output1 + 8, output1 + 8 + 16, keys.appKey);

  SimpleLoRaWAN::Pinmapping pins = { PA_7, PA_6, PA_5, PB_0, PB_1, PA_8, PA_9 };
  
  {
    USBSerial serial(false);
    serial.connect();

    for(int i = 0; i < 10; i++) {

      if(serial.ready()) {
        serial.printf("\r\n*** Starting LoRaWAN Micro Weather Station ***\r\n");

        // serial.printf("\r\n\r\n");
        serial.printf("appEui: ");
        for(int i = 0; i < 8; i++) {
          serial.printf("%02X ", keys.appEui[i]);
        }
        serial.printf("\r\n");

        serial.printf("devEui: ");
        for(int i = 0; i < 8; i++) {
          serial.printf("%02X ", keys.devEui[i]);
        }
        serial.printf("\r\n");

        serial.printf("appKey: ");
        for(int i = 0; i < 16; i++) {
          serial.printf("%02X ", keys.appKey[i]);
        }
        serial.printf("\r\n");

        break;
      }
      serial.printf(".");

      led = 1;
      ThisThread::sleep_for(10ms);
      led = 0;
      ThisThread::sleep_for(990ms);
    }
  }

  Node node(keys, pins);
  
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