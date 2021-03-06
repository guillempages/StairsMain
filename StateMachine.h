#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "config.h"
#include "SingleStep.h"

class State;
class Tcp;
class Spi;

class StateMachine {
public:
    StateMachine(State * initialState);
    virtual ~StateMachine();

    void setSpi(Spi* _spi) {spi = _spi;}
    void setTcp(Tcp* _tcp) {tcp = _tcp;}

    Spi* getSpi() {return spi;}
    Tcp* getTcp() {return tcp;}

    void tick();
    void transition(State * newState);

    virtual uint8_t getStepCount() { return stepCount;};
    virtual void setMode(uint8_t mode);
    virtual uint8_t getMode() {return currentMode;};
    void setStepCount(uint8_t count);
    virtual SingleStep& getStep(uint8_t index);

    char currentStateName[64];
    uint8_t stepCount;

    uint8_t runningLightId;
    uint8_t runningLightDirection;

    uint8_t previousMode;
    uint8_t currentMode;

private:
    State * currentState;
    Tcp* tcp;
    Spi* spi;
    SingleStep * steps;
    SingleStep dummyStep;
};


class TcpStateMachine : public StateMachine{
public:
    TcpStateMachine(State * initialState, StateMachine* spiMachine);
    virtual ~TcpStateMachine() {}

    virtual uint8_t getStepCount() {return spiStateMachine->getStepCount();}
    virtual SingleStep& getStep(uint8_t index) {return spiStateMachine->getStep(index);}
    virtual void setMode(uint8_t mode) {spiStateMachine->setMode(mode);}
    virtual uint8_t getMode() {return spiStateMachine->getMode();}
private:
    StateMachine * spiStateMachine;
};

class State {
public:
    State(String name);
    virtual ~State() {}
    void setStateMachine(StateMachine* parent);

    virtual void enter() {}
    virtual void process() {}
    virtual void exit() {}
    inline String getName() {return name;};

protected:
    StateMachine* parent;

private:
    String name;
};

#endif //_STATE_MACHINE_H
