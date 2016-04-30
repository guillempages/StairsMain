#ifndef _IPCONNECTION_H_
#define _IPCONNECTION_H_

#include <spark_wiring_tcpclient.h>
#include <spark_wiring_tcpserver.h>

class RingBuffer {
public:
    RingBuffer(unsigned int size);
    virtual ~RingBuffer();

    void put(uint8_t character);
    uint8_t get();
    inline unsigned int size() { return (last - first + maxSize) % maxSize;};
    inline void clear() { first = last = 0; };

private:
    uint8_t *buffer;
    unsigned int maxSize;
    unsigned int first;
    unsigned int last;
};

class Tcp {
public:
    Tcp(unsigned int port);

    inline bool isConnected() { return client.connected(); };
    void tick();
    String read();
    void write(String value);
    void disconnect();

private:
    TCPServer server;
    TCPClient client;
    bool connected;
    RingBuffer sendBuffer;
    RingBuffer recvBuffer;

};

#endif //_IPCONNECTION_H_
