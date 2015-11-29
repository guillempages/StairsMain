
#include "config.h"
#include "commands.h"

#include "ipconnection.h"
#include "spi.h"

#include "StateMachine.h"
#include "SpiStates.h"
#include "SingleStep.h"
#include "TcpStates.h"

char buffer[20];
int lastSpi;
int command;

StateMachine * stateMachineSpi;
TcpStateMachine * stateMachineTcp;

Spi* spi;

void setup() {
    pinMode(connectedLed, OUTPUT);

    pinMode(mosi, OUTPUT);
    pinMode(sck, OUTPUT);
    pinMode(miso, INPUT);
    pinMode(ss, OUTPUT);

    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(A6, OUTPUT);
    pinMode(A7, OUTPUT);


    spi = new Spi();

    stateMachineSpi = new StateMachine(new Init1State());
    stateMachineSpi->setSpi(spi);

    stateMachineTcp = new TcpStateMachine(new TcpDisconnected(), stateMachineSpi);

    command = CMD_LED_FADE;

//    Spark.variable("steps", &stateMachineSpi->stepCount, INT);
    Spark.variable("spiState", stateMachineSpi->currentStateName, STRING);
    Spark.variable("tcpState", stateMachineTcp->currentStateName, STRING);

}


void loop() {
    stateMachineSpi->tick();
    stateMachineTcp->tick();
//        digitalWrite(A7, HIGH);
//        delay(1);
//        digitalWrite(A7, LOW);
}
