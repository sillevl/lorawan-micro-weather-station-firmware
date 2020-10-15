#include "KeyFactory.h"
#include "mbedtls/sha256.h" /* SHA-256 only */

KeyFactory::KeyFactory(uint8_t* appEui) {

  std::copy(appEui, appEui + 8, this->appEui);

  uint8_t* uid_reg = (uint8_t*)0x1FFF7590;
  std::copy(uid_reg, uid_reg + 12, uid);

  unsigned char hash[32]; /* SHA-256 outputs 32 bytes */
  mbedtls_sha256( (const unsigned char*) uid, 12, hash, 0);

  std::copy(appEui, appEui + 8, keys.appEui);
  std::copy(hash + 0, hash + 8, keys.devEui);
  std::copy(hash + 8, hash + 8 + 16, keys.appKey);
}

SimpleLoRaWAN::LoRaWANKeys KeyFactory::getKeys() {
  return keys;
}

uint8_t* KeyFactory::getUID() {
  return uid;
}

void KeyFactory::printKeys(USBSerial* serial) {
  // serial->printf("\r\n\r\n");

  serial->printf("uid:    ");
  for(int i = 0; i < 12; i++) {
    serial->printf("%02X ", uid[i]);
  }
  serial->printf("\r\n");

  serial->printf("appEui: ");
  for(int i = 0; i < 8; i++) {
    serial->printf("%02X ", keys.appEui[i]);
  }
  serial->printf("\r\n");

  serial->printf("devEui: ");
  for(int i = 0; i < 8; i++) {
    serial->printf("%02X ", keys.devEui[i]);
  }
  serial->printf("\r\n");

  serial->printf("appKey: ");
  for(int i = 0; i < 16; i++) {
    serial->printf("%02X ", keys.appKey[i]);
  }
  serial->printf("\r\n");
}
