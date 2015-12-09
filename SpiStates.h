#ifndef _SPI_STATES_H
#define _SPI_STATES_H
#include "config.h"
#include "StateMachine.h"
#include "SingleStep.h"

class SpiState : public State {
public:
    SpiState(String name, SingleStep::StepValues value=SingleStep::Dummy) : State(name), count(0), receiveValue(value) {};
    void process();

protected:
    uint8_t count;
    SingleStep::StepValues receiveValue;

};

class Init1State : public SpiState {
public:
    Init1State(): SpiState("init1") {};
    void process();
};

class Init2State : public SpiState {
public:
    Init2State() : SpiState("init2") {};
    void process();
};

class ReadIrState : public SpiState {
public:
    ReadIrState(SingleStep::StepValues value = SingleStep::Dummy) : SpiState("readIr", value) {};
    void process();
};

class ReadLightState : public SpiState {
public:
    ReadLightState(SingleStep::StepValues value = SingleStep::Dummy) : SpiState("readLight", value) {};
    void process();
};

class ExecuteState : public SpiState {
public:
    ExecuteState(SingleStep::StepValues value = SingleStep::Dummy) : SpiState("execute", value) {};
    void process();
};

/////////////
class OnState : public SpiState {
public:
    OnState(SingleStep::StepValues value = SingleStep::Dummy): SpiState("on", value) {};
    void process();
};

class OffState : public SpiState {
public:
    OffState(SingleStep::StepValues value = SingleStep::Dummy): SpiState("off", value) {};
    void process();
};

class OnOffState : public SpiState {
public:
    OnOffState(SingleStep::StepValues value = SingleStep::Dummy): SpiState("onoff", value), loopCount(0) {};
    void process();
private:
    uint8_t loopCount;
};

class OffOnState : public SpiState {
public:
    OffOnState(SingleStep::StepValues value = SingleStep::Dummy): SpiState("offon", value), loopCount(0) {};
    void process();
  private:
      uint8_t loopCount;
};

#endif //_SPI_STATES_H
