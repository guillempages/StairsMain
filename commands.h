#ifndef COMMANDS_H
#define COMMANDS_H

#define CMD_NOOP          0x00

#define CMD_INIT_1        0x05  //This command will be forwarded as such; can be used to check the number of slaves.
#define CMD_INIT_2        0x0C  //This command will be forwarded as such; can be used to check the number of slaves.

#define CMD_LED_FADE      0x20  //Fade the LED slowly on or off, depending on the last bit.
#define CMD_LED_SET       0x30  //Commands to switch the LED on or off, depending on the last bit.

#define CMD_READ_SENSOR   0x10  //Read the current value of the desired ADC converter. Last 4 bits reserved for indexing.
#define CMD_CHECK_SENSOR  0x60  //Compare the sensor with the current threshold; return 0 if lower, 0xFF if higher. Last 4 bits reserved for indexing.
#define CMD_GET_THRESHOLD 0x50  //Get the current threshold for the given sensor. Last 4 bits reserved for indexing.
#define CMD_SET_THRESHOLD 0x40  //Set the current threshold for the given sensor. Last 4 bits reserved for indexing.
                                      //The next "executable command" will be interpreted as the threshold and not executed.

#define CMD_TEST_MODE     0xD0  //Restart the test mode.
/* MASKS */
#define CMD_COMMAND_MASK  0xF0  // Mask used to separate the command from the parameter.
#define CMD_PARAM_MASK    0x0F  // Mask used to separate parameter from the command.

/* Convenience commands; with parameter included */
#define CMD_LED_SET_OFF   CMD_LED_SET        //Switch the LED off, regardless of the current status.
#define CMD_LED_SET_ON    (CMD_LED_SET | 0x08)  //Switch the LED on, regardless of the current status.

#define CMD_LED_FADE_OFF  CMD_LED_FADE       //Fade the LED slowly off.
#define CMD_LED_FADE_ON   (CMD_LED_FADE | 0x08) //Fade the LED slowly on.

#define CMD_GET_STEP        (CMD_READ_SENSOR | SENSOR_STEP) // Get the value of the (step) sensor ADC converter (ADC 2)
#define CMD_GET_LIGHT       (CMD_READ_SENSOR | SENSOR_LIGHT) //Get the value of the (light) sensor ADC converter (ADC 0)
#define CMD_GET_TEMPERATURE (CMD_READ_SENSOR | SENSOR_TEMPERATURE) //Get the value of the internal temperature sensor (ADC 8)

#define SENSOR_STEP         2
#define SENSOR_LIGHT        7
#define SENSOR_TEMPERATURE  8

#endif //COMMANDS_H
