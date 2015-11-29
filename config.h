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

#endif //_MY_CONFIG_H_
