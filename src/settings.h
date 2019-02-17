#pragma once

#include "config.h"

SimpleLoRaWAN::Config config = {
    { 
        { 0x00, 0xA7, 0x29, 0x22, 0xB2, 0x4C, 0xC2, 0x11 },         // devEui
        { 0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x3C, 0x10 },         // appEui
        { 0xF3, 0x1A, 0xE7, 0xFA, 0xB3, 0x8F, 0x3B, 0x36, 0x69, 0x30, 0xC2, 0xBE, 0x9D, 0x8A, 0x8E, 0x35 } // appKey
    },
    { D11, D12, D13, A0, A1, D2, D3 }  // miso, mosi, clk, nss, reset, dio0, dio1
    ,
    true // wait until connect
};