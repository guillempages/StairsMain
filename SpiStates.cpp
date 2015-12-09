#include "SpiStates.h"
#include "commands.h"
#include "spi.h"
#include "SingleStep.h"


void showState(uint8_t state) {
    digitalWrite(A3, state & 4);
    digitalWrite(A4, state & 2);
    digitalWrite(A5, state & 1);
}

void SpiState::process() {
    parent->getStep(count).values[receiveValue] = parent->getSpi()->receive();
}

void Init1State::process() {
    showState(1);
    parent->getSpi()->send(CMD_INIT_1);
    if (parent->getSpi()->receive() == CMD_INIT_1) {
        parent->getSpi()->execute(); // Sync all steps.
        parent->transition(new Init2State());
    }
}

void Init2State::process() {
    showState(2);
    parent->getSpi()->send(CMD_INIT_2);
    if (parent->getSpi()->receive() == CMD_INIT_2) {
        parent->getSpi()->execute(); // Sync all steps.
        parent->setStepCount(count);
        parent->transition(new OnOffState());
    } else {
        count++;
    }
}

void ReadIrState::process() {
    showState(3);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new ReadIrState(SingleStep::IrValue));
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_GET_STEP);
        ++count;
    }
}

void ReadLightState::process() {
    showState(4);
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
    showState(5);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new OnState(SingleStep::IrThreshold));
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_GET_TEMPERATURE);
        ++count;
    }
}

void OnState::process() {
    showState(6);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new OffState(SingleStep::LightThreshold));
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_LED_FADE_ON);
        ++count;
    }
}

void OffState::process() {
    showState(7);
    if (count > parent->getStepCount()) {
        parent->getSpi()->execute();
        parent->transition(new ReadIrState());
    } else {
        SpiState::process();
        parent->getSpi()->send(CMD_LED_FADE_OFF);
        ++count;
   }
}

void OnOffState::process() {
    showState(8);
    if (count > parent->getStepCount()) {
        loopCount++;
        if (loopCount >= ONOFF_LOOP_COUNT) {
            parent->getSpi()->execute();
            parent->transition(new OffOnState(SingleStep::LightThreshold));
          } else {
            count = 0;
          }
    } else {
        SpiState::process();
        if (count % 2) {
          parent->getSpi()->send(CMD_LED_FADE_ON);
        } else {
          parent->getSpi()->send(CMD_LED_FADE_OFF);
        }
        ++count;
    }
}

void OffOnState::process() {
    showState(9);
    if (count > parent->getStepCount()) {
        loopCount++;
        if (loopCount >= ONOFF_LOOP_COUNT) {
            parent->getSpi()->execute();
            parent->transition(new OnOffState(SingleStep::LightThreshold));
          } else {
            count = 0;
          }
    } else {
        SpiState::process();
        if (count % 2) {
          parent->getSpi()->send(CMD_LED_FADE_OFF);
        } else {
          parent->getSpi()->send(CMD_LED_FADE_ON);
        }
        ++count;
    }
}
