#ifndef _SPI_H_
#define _SPI_H_
#include "config.h"

class Spi {

public:
    Spi();

    void execute();

    void send(uint8_t command);
    uint8_t receive();

protected:
    uint8_t getMsb(uint8_t value);

private:
    uint8_t lastReceived;
    uint8_t ssStatus;
};


inline uint8_t Spi::receive() {
    return lastReceived;
}

/**
 * Return the current MSB from the requested value. Useful to set pins to HIGH or LOW.
 */
inline uint8_t Spi::getMsb(uint8_t value) {
    if (value & 0x80) {
        return HIGH;
    } else {
        return LOW;
    }
}

#endif //_SPI_H_
