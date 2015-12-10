#include <spark_wiring.h>

#include "config.h"
#include "spi.h"

Spi::Spi() : lastReceived(0) {
    ssStatus = digitalRead(ss);
}

void Spi::execute() {
    digitalWrite(ss,HIGH);
    delay(SPI_EXEC_DELAY);
    digitalWrite(ss, LOW);
    digitalWrite(sck, LOW);
}

/**
* Send an SPI command out, and read the incoming values.
*/
void Spi::send(uint8_t command) {
    lastReceived = 0;

    for (uint8_t i = 0; i<8; i++) {
        delay(SPI_BIT_DELAY);
        digitalWrite(sck, LOW);
        digitalWrite(mosi, getMsb(command));
        delay(SPI_BIT_DELAY);
        command <<= 1;
        lastReceived <<= 1;
        if (digitalRead(miso) == HIGH) {
            lastReceived |= 1;
        }
        digitalWrite(sck, HIGH);
    }
}
