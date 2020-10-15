#pragma once

#include "Simple-LoRaWAN.h"
#include "mbed.h"
#include "USBSerial.h"

class KeyFactory {
  public:
  KeyFactory(uint8_t*);
  SimpleLoRaWAN::LoRaWANKeys getKeys();
  void printKeys(USBSerial*);
  uint8_t* getUID();

  private:
  uint8_t devEui[8];
  uint8_t appEui[8];
  uint8_t appKey[16];
  uint8_t uid[12];
  SimpleLoRaWAN::LoRaWANKeys keys;
};