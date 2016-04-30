#ifndef NET_COMMANDS_H
#define NET_COMMANDS_H

#define NET_CMD_NOOP 0x00

#define NET_CMD_CHANGE_MODE 0x10


// Modes:
#define MODE_IDLE 0x00 // Stop sending SPI commands.
#define MODE_NORMAL 0x01 // Read sensors and change lights accordingly
#define MODE_ON  0x02 // Set all lights on; ignore sensors.
#define MODE_OFF 0x03 // Set all lights off; ignore sensors.
#define MODE_BLINK 0x04 // Switch lights on and off alternating.
#define MODE_BLINK_ALT 0x05 // Blink lights alternating
#define MODE_RUNNING_LIGHT 0x06 // Move a single LED up and down.
#define MODE_RUNNING_HOLE 0x07 // Light all LEDS except one and move that up and down.
#define MODE_LAST_MODE MODE_RUNNING_HOLE

#endif NET_COMMANDS_H
