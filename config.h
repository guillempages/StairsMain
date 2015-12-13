#ifndef _MY_CONFIG_H_
#define _MY_CONFIG_H_

#include <spark_wiring.h>

// Outputs
const int led0 = A2;
const int connectedLed = D7;

const int ss = D3;
const int sck = D4;
const int miso = D5;
const int mosi = D6;

const int tcpPort = 2222;
const int bufferSize = 255;
const int sendLength = 20;

const bool HUMAN = false;

const uint8_t SPI_EXEC_DELAY = 2;
const uint8_t SPI_BIT_DELAY = 0;

const int IDLE_DELAY = 200;
const int ONOFF_DELAY = 500;
const int RUNNING_LIGHT_DELAY = 300;

#endif //_MY_CONFIG_H_
