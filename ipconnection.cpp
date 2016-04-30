#include <spark_wiring.h>
#include "ipconnection.h"
#include "config.h"

RingBuffer::RingBuffer(unsigned int size) : maxSize(size + 1), first(0), last(0) {
    buffer = new uint8_t[maxSize]();
}

RingBuffer::~RingBuffer() {
    delete [] buffer;
}

void RingBuffer::put(uint8_t character) {
    buffer[last] = character;

    ++last %= maxSize;

    if (last == first) {
        ++first %= maxSize;
    }
}

uint8_t RingBuffer::get() {
    uint8_t result = buffer[first];
    ++first %= maxSize;
    return result;
}




Tcp::Tcp(unsigned int port) : server(port), sendBuffer(bufferSize), recvBuffer(bufferSize) {
    server.begin();
}

void Tcp::tick() {
    if (!isConnected()) {
        client.stop();
        client = server.available();
        recvBuffer.clear();
    } else {
        unsigned int count = client.available();
        for (unsigned int i = 0; i < count; i++) {
            recvBuffer.put(client.read());
        }
        for (unsigned int i = min(sendLength, sendBuffer.size()); i > 0 ; i--) {
            client.write(sendBuffer.get());
        }
    }
}

String Tcp::read() {
    String result="";
    unsigned int length = recvBuffer.size();
    result.reserve(length);
    for (unsigned int i = 0; i < length; i++) {
        result += (char)recvBuffer.get();
    }
    return result;
}

void Tcp::write(String value) {
    for (unsigned int i = 0; i < value.length(); i++) {
        sendBuffer.put(value.charAt(i));
    }
}

void Tcp::disconnect() {
    client.stop();
}
