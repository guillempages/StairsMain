#include "StateMachine.h"
#include "ipconnection.h"
#include "net_commands.h"

StateMachine::StateMachine(State * initialState) :
        stepCount(0),
        runningLightId(-1),
        runningLightDirection(0),
        previousMode(MODE_IDLE),
        currentMode(MODE_NORMAL),
        currentState(initialState),
        steps(0) {
    currentState->getName().toCharArray(currentStateName, 63);
    currentState->setStateMachine(this);
    currentState->enter();
}

StateMachine::~StateMachine() {
    delete currentState;
    delete [] steps;
}

void StateMachine::tick() {
    currentState->process();
}

void StateMachine::transition(State* newState) {
    currentState->exit();
    delete currentState;
    currentState = newState;
    currentState->getName().toCharArray(currentStateName, 63);
    currentState->setStateMachine(this);
    currentState->enter();
}

void StateMachine::setStepCount(uint8_t count) {
    stepCount = count;
    delete [] steps;
    steps = new SingleStep[stepCount];
}

void StateMachine::setMode(uint8_t mode) {
    if (mode >= 0 && mode < MODE_LAST_MODE) {
        currentMode = mode;
    }
}

TcpStateMachine::TcpStateMachine(State* initialState, StateMachine* spiMachine) :
    StateMachine(initialState),
    spiStateMachine(spiMachine)
{
    setTcp(new Tcp(tcpPort));
}

SingleStep& StateMachine::getStep(uint8_t index) {
    if (steps == 0 || index < 0 || index >= stepCount) {
        return dummyStep;
    } else {
        return steps[index];
    }
}

State::State(String _name) : parent(0), name(_name) {

}

void State::setStateMachine(StateMachine* _parent) {
    parent = _parent;
}
