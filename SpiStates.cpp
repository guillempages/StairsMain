#include "SpiStates.h"
#include "commands.h"
#include "spi.h"
#include "SingleStep.h"
#include "net_commands.h"


void showState(uint8_t state) {
    digitalWrite(A3, state & 4);
    digitalWrite(A4, state & 2);
    digitalWrite(A5, state & 1);
}

void SpiState::process() {
    parent->getStep(count).values[receiveValue] = parent->getSpi()->receive();
}

void Init1State::process() {
//    showState(1);
    parent->getSpi()->send(CMD_INIT_1);
    if (parent->getSpi()->receive() == CMD_INIT_1) {
        parent->getSpi()->execute(); // Sync all steps.
        parent->transition(new Init2State());
    }
}

void Init2State::process() {
//    showState(2);
    parent->getSpi()->send(CMD_INIT_2);
    if (parent->getSpi()->receive() == CMD_INIT_2) {
        parent->getSpi()->execute(); // Sync all steps.
        parent->setStepCount(count);
        parent->transition(new CheckModeState());
    } else {
        count++;
    }
}

void ReadIrState::process() {
//    showState(3);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new ReadLightState(SingleStep::IrValue));
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_GET_STEP);
        ++count;
    }
}

void ReadLightState::process() {
//    showState(4);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new ExecuteState(SingleStep::LightValue));
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_GET_LIGHT);
        ++count;
    }
}

void ExecuteState::process() {
//    showState(5);
    if (count > parent->getStepCount()) {
        updateRunningLights();
        parent->getSpi()->execute();
        parent->transition(new CheckModeState(SingleStep::IrThreshold));
    } else {
        SpiState::process();
        if (count == parent->runningLightId) {
            parent->getSpi()->send(CMD_LED_SET_ON);
        } else {
            parent->getSpi()->send(CMD_LED_SET_OFF);
        }
        ++count;
    }
}

void ExecuteState::updateRunningLights() {
    parent->runningLightId += parent->runningLightDirection;
    if (parent->runningLightDirection != 0 &&
            (parent->runningLightId > parent->getStepCount() || parent->runningLightId <= 0)) {
        parent->runningLightId = -1;
        parent->runningLightDirection = 0;
    }
}

void OnState::process() {
//    showState(6);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_LED_FADE_ON);
        ++count;
    }
}

void OffState::process() {
//    showState(7);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_LED_FADE_OFF);
        ++count;
   }
}

void OnOffState::process() {
//    showState(8);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        if (count % 2) {
          parent->getSpi()->send(CMD_LED_FADE_ON);
        } else {
          parent->getSpi()->send(CMD_LED_FADE | 0x07);
        }
        ++count;
    }
}

void OffOnState::process() {
//    showState(9);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        if (count % 2) {
          parent->getSpi()->send(CMD_LED_FADE | 0x07);
        } else {
          parent->getSpi()->send(CMD_LED_FADE_ON);
        }
        ++count;
    }
}

void RunningLightState::process() {
//    showState(5);
    if (count > parent->getStepCount()) {
        updateRunningLights();
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        if (count == parent->runningLightId) {
            parent->getSpi()->send(CMD_LED_FADE_ON);
        } else {
            parent->getSpi()->send(CMD_LED_FADE_OFF);
        }
        ++count;
    }
}

void RunningLightState::updateRunningLights() {
    if (parent->runningLightId >= parent->getStepCount()) {
        parent->runningLightDirection = -1;
    } else if (parent->runningLightId <= 1) {
        parent->runningLightDirection = 1;
    }
    parent->runningLightId += parent->runningLightDirection;
}

void RunningHoleState::process() {
//    showState(5);
    if (count > parent->getStepCount()) {
        updateRunningLights();
        parent->getSpi()->execute();
        parent->transition(new CheckModeState());
    } else {
        SpiState::process();
        if (count == parent->runningLightId) {
            parent->getSpi()->send(CMD_LED_FADE_OFF);
        } else {
            parent->getSpi()->send(CMD_LED_FADE_ON);
        }
        ++count;
    }
}

void RunningHoleState::updateRunningLights() {
    if (parent->runningLightId >= parent->getStepCount()) {
        parent->runningLightDirection = -1;
    } else if (parent->runningLightId <= 1) {
        parent->runningLightDirection = 1;
    }
    parent->runningLightId += parent->runningLightDirection;
}

void CheckModeState::process() {
    uint8_t currentMode = parent->currentMode;
    showState(currentMode);
    switch (currentMode) {
        case MODE_NORMAL:
            if (parent->previousMode != currentMode) {
                parent->runningLightId = -1;
                parent->runningLightDirection = 0;
            }
            if (parent->previousMode == MODE_IDLE) {
                parent->transition(new Init1State());
            } else {
                parent->transition(new ReadIrState(receiveValue));
            }
            break;
        case MODE_ON:
            parent->transition(new OnState(receiveValue));
            break;
        case MODE_OFF:
            parent->transition(new OffState(receiveValue));
            break;
        case MODE_BLINK:
            if (parent->previousMode == MODE_OFF) {
                parent->transition(new OnState(receiveValue));
                currentMode = MODE_ON;
            } else {
                parent->transition(new OffState(receiveValue));
                currentMode = MODE_OFF;
            }
            delay(ONOFF_DELAY);
            break;
        case MODE_BLINK_ALT:
            if (parent->previousMode == MODE_OFF) {
                parent->transition(new OffOnState(receiveValue));
                currentMode = MODE_ON;
            } else {
                parent->transition(new OnOffState(receiveValue));
                currentMode = MODE_OFF;
            }
            delay(ONOFF_DELAY);
            break;
        case MODE_RUNNING_LIGHT:
            if (parent->previousMode != currentMode) {
                parent->runningLightId = 0;
                parent->runningLightDirection = 1;
            }
            parent->transition(new RunningLightState(receiveValue));
            delay(RUNNING_LIGHT_DELAY);
            break;
        case MODE_RUNNING_HOLE:
            if (parent->previousMode != currentMode) {
                parent->runningLightId = 0;
                parent->runningLightDirection = 1;
            }
            parent->transition(new RunningHoleState(receiveValue));
            delay(RUNNING_LIGHT_DELAY);
            break;
        case MODE_IDLE:
        default:
            delay(IDLE_DELAY);
    }

    parent->previousMode = currentMode;
}
