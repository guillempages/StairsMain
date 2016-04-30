
#include "config.h"
#include "commands.h"
#include "net_commands.h"

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

int currentMode;

int toggleMode(String newMode) {
    int8_t currentMode = stateMachineSpi->getMode() + 1;
    if (currentMode > MODE_LAST_MODE) {
        currentMode = 0;
    }
    stateMachineSpi->setMode(currentMode);
    return stateMachineSpi->getMode();
}

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

//    Particle.variable("steps", stateMachineSpi->stepCount);
    Particle.variable("currentMode", currentMode);
    Particle.variable("spiState", stateMachineSpi->currentStateName);
    Particle.variable("tcpState", stateMachineTcp->currentStateName);
    Particle.function("toggleMode", toggleMode);

}

void loop() {
    stateMachineSpi->tick();
    stateMachineTcp->tick();
    currentMode = stateMachineSpi->getMode();
//        digitalWrite(A7, HIGH);
//        delay(1);
//        digitalWrite(A7, LOW);
}
