#ifndef _TCP_STATES_H_
#define _TCP_STATES_H_

#include "StateMachine.h"

class TcpDisconnected : public State {
public:
    TcpDisconnected() : State("disconnected") {};
    void enter();
    void process();
};

class TcpConnected : public State {
public:
    TcpConnected() : State("connected"), count(0) {};
    void enter();
    void process();
private:
    uint8_t count;

    void writeStateHuman(const SingleStep& step);
    void writeStateBinary(const SingleStep& step);
};

#endif //_TCP_STATES_H_
