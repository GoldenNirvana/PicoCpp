#ifndef AD56X4_h
#define AD56X4_h

#define AD56X4_COMMAND_WRITE_INPUT_REGISTER            0b00000000
#define AD56X4_COMMAND_UPDATE_DAC_REGISTER             0b00001000
#define AD56X4_COMMAND_WRITE_INPUT_REGISTER_UPDATE_ALL 0b00010000
#define AD56X4_COMMAND_WRITE_UPDATE_CHANNEL            0b00011000
#define AD56X4_COMMAND_POWER_UPDOWN                    0b00100000
#define AD56X4_COMMAND_RESET                           0b00101000
#define AD56X4_COMMAND_SET_LDAC                        0b00110000
#define AD56X4_COMMAND_REFERENCE_ONOFF                 0b00111000

#define AD56X4_CHANNEL_A                              0b00000000
#define AD56X4_CHANNEL_B                              0b00000001
#define AD56X4_CHANNEL_C                              0b00000010
#define AD56X4_CHANNEL_D                              0b00000011
#define AD56X4_CHANNEL_ALL                            0b00000111

#define AD56X4_SETMODE_INPUT                           AD56X4_COMMAND_WRITE_INPUT_REGISTER
#define AD56X4_SETMODE_INPUT_DAC                       AD56X4_COMMAND_WRITE_UPDATE_CHANNEL
#define AD56X4_SETMODE_INPUT_DAC_ALL                   AD56X4_COMMAND_WRITE_INPUT_REGISTER_UPDATE_ALL

#define AD56X4_POWERMODE_NORMAL                       0b00000000
#define AD56X4_POWERMODE_POWERDOWN_1K                 0b00010000
#define AD56X4_POWERMODE_POWERDOWN_100K               0b00100000
#define AD56X4_POWERMODE_TRISTATE                     0b00110000

#include <cstdint>

#define word uint16_t
#define byte uint8_t
#define boolean bool


class AD56X4Class {

public:

    static void setChannel(byte setMode, byte channel,
                           word value);

    static void setChannel(int SS_pin, byte setMode, word values[]);

    static void setChannel(int SS_pin, byte setMode, word value_D,
                           word value_C, word value_B, word value_A);

    static void updateChannel(byte channel);

    static void powerUpDown(int SS_pin, byte powerMode,
                            boolean channels[]);

    static void powerUpDown(int SS_pin, byte powerMode,
                            boolean channel_D, boolean channel_C,
                            boolean channel_B, boolean channel_A);

    static void powerUpDown(int SS_pin, byte powerModes[]);

    static void reset(int SS_pin, boolean fullReset);

    static void setInputMode(int SS_pin, boolean channels[]);

    static void setInputMode(int SS_pin, boolean channel_D,
                             boolean channel_C, boolean channel_B,
                             boolean channel_A);

    static void useInternalReference(int SS_pin, boolean yesno);

private:
    inline static void powerUpDown(int SS_pin, byte powerMode,
                                   byte channelMask);

    inline static void setInputMode(int SS_pin, byte channelMask);

    static word makeChannelMask(boolean channels[]);

    static word makeChannelMask(boolean channel_D, boolean channel_C,
                                boolean channel_B, boolean channel_A);

    static void writeMessage(int SS_pin, byte command, byte address,
                             word data);

};

extern AD56X4Class AD56X4;

#endif 