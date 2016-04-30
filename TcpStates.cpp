#include "config.h"
#include "net_commands.h"
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
        parent->getTcp()->write(String(" "));
        parent->getTcp()->write(String(parent->getMode()));
    } else {
        parent->getTcp()->write(String((char) parent->getStepCount()));
        parent->getTcp()->write(String((char) parent->getMode()));
    }
    parent->getTcp()->write(String('\n'));
}

void TcpConnected::process() {
    parent->getTcp()->tick();
    if (parent->getTcp()->isConnected()) {
        String readValue = parent->getTcp()->read();
        if (readValue.length() >= 2) {
            switch (readValue[0]) {
                case NET_CMD_CHANGE_MODE:
                    parent->setMode(readValue[1]);
                    break;
                case NET_CMD_DISCONNECT:
                    parent->getTcp()->disconnect();
                    parent->transition(new TcpDisconnected());
                    break;
            }
        }
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
    String singleStep("01234\n");

    uint8_t index = 0;
    singleStep.setCharAt(index++, count);
    singleStep.setCharAt(index++, step.values[SingleStep::IrValue]);
    singleStep.setCharAt(index++,step.values[SingleStep::LightValue]);
    singleStep.setCharAt(index++,step.values[SingleStep::IrThreshold]);
    singleStep.setCharAt(index++,step.values[SingleStep::LightThreshold]);
    singleStep.setCharAt(index++,'\n');

    parent->getTcp()->write(singleStep);
}
