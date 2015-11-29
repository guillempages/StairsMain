#include "config.h"
#include "TcpStates.h"
#include "ipconnection.h"
#include "SingleStep.h"

void TcpDisconnected::enter() {
    digitalWrite(connectedLed, LOW);
}

void TcpDisconnected::process() {
    parent->getTcp()->tick();
    if (parent->getTcp()->isConnected()) {
        parent->transition(new TcpConnected());
    }
}

void TcpConnected::enter() {
    digitalWrite(connectedLed, HIGH);
    if (HUMAN) {
        parent->getTcp()->write(String(parent->getStepCount()));
    } else {
        parent->getTcp()->write(String((char) parent->getStepCount()));
    }
    parent->getTcp()->write(String('\n'));
}

void TcpConnected::process() {
    parent->getTcp()->tick();
    if (parent->getTcp()->isConnected()) {
        if (parent->getStepCount() > 0) {
            SingleStep step = parent->getStep(count);
            if (HUMAN) {
                writeStateHuman(step);
            } else {
                writeStateBinary(step);
            }
            ++count %= parent->getStepCount();
        }
    } else {
        parent->transition(new TcpDisconnected());
    }
}

void TcpConnected::writeStateHuman(const SingleStep& step) {
    parent->getTcp()->write(String(count));
    parent->getTcp()->write(String(' '));
    parent->getTcp()->write(String(step.values[SingleStep::IrValue], HEX));
    parent->getTcp()->write(String(' '));
    parent->getTcp()->write(String(step.values[SingleStep::LightValue], HEX));
    parent->getTcp()->write(String(' '));
    parent->getTcp()->write(String(step.values[SingleStep::IrThreshold], HEX));
    parent->getTcp()->write(String(' '));
    parent->getTcp()->write(String(step.values[SingleStep::LightThreshold], HEX));
    parent->getTcp()->write(String('\n'));
}

void TcpConnected::writeStateBinary(const SingleStep& step) {
    String singleStep("00000\n");

    singleStep.setCharAt(0, count);
    singleStep.setCharAt(1, step.values[SingleStep::IrValue]);
    singleStep.setCharAt(2,step.values[SingleStep::LightValue]);
    singleStep.setCharAt(3,step.values[SingleStep::IrThreshold]);
    singleStep.setCharAt(4,step.values[SingleStep::LightThreshold]);
    singleStep.setCharAt(5,'\n');

    parent->getTcp()->write(singleStep);
}
