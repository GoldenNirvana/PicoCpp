#include "ad5664.hpp"
#include "../Spi.hpp"
#include "../utilities/io_ports.h"

AD56X4Class AD56X4;


void AD56X4Class::setChannel(byte setMode, byte channel,
                             word value) {
    if (setMode == AD56X4_SETMODE_INPUT
        || setMode == AD56X4_SETMODE_INPUT_DAC
        || setMode == AD56X4_SETMODE_INPUT_DAC_ALL)
        AD56X4.writeMessage(0, setMode, channel, value);
}

void AD56X4Class::setChannel(int SS_pin, byte setMode, word values[]) {
    if (setMode == AD56X4_SETMODE_INPUT
        || setMode == AD56X4_SETMODE_INPUT_DAC
        || setMode == AD56X4_SETMODE_INPUT_DAC_ALL) {
        for (int i = 3; i >= 0; i--)
            AD56X4.writeMessage(SS_pin, setMode, i, values[3 - i]);
    }
}

void AD56X4Class::setChannel(int SS_pin, byte setMode, word value_D,
                             word value_C, word value_B, word value_A) {
    word values[] = {value_D, value_C, value_B, value_A};
    AD56X4.setChannel(SS_pin, setMode, values);
}

void AD56X4Class::updateChannel(byte channel) {
    AD56X4.writeMessage(0, AD56X4_COMMAND_UPDATE_DAC_REGISTER, channel, 0);
}

void AD56X4Class::powerUpDown(int SS_pin, byte powerMode,
                              byte channelMask) {
    AD56X4.writeMessage(SS_pin, AD56X4_COMMAND_POWER_UPDOWN, 0,
                        (word) ((0b00110000 & powerMode)
                                | (0b00001111 & channelMask)));
}

void AD56X4Class::powerUpDown(int SS_pin, byte powerMode,
                              boolean channels[]) {
    AD56X4.powerUpDown(SS_pin, powerMode,
                       AD56X4.makeChannelMask(channels));
}

void AD56X4Class::powerUpDown(int SS_pin, byte powerMode,
                              boolean channel_D, boolean channel_C,
                              boolean channel_B, boolean channel_A) {
    AD56X4.powerUpDown(SS_pin, powerMode,
                       AD56X4.makeChannelMask(channel_D, channel_C,
                                              channel_B, channel_A));
}

void AD56X4Class::powerUpDown(int SS_pin, byte powerModes[]) {
    byte channelMask = 1;
    for (int i = 0; i < 4; i++) {
        AD56X4.powerUpDown(SS_pin, powerModes[i], channelMask);
        channelMask = channelMask << 1;
    }
}

void AD56X4Class::reset(int SS_pin, boolean fullReset) {
    AD56X4.writeMessage(SS_pin, AD56X4_COMMAND_RESET, 0, (word) fullReset);
}


void AD56X4Class::setInputMode(int SS_pin, byte channelMask) {
    AD56X4.writeMessage(SS_pin, AD56X4_COMMAND_SET_LDAC, 0,
                        (word) channelMask);
}

void AD56X4Class::setInputMode(int SS_pin, boolean channels[]) {
    AD56X4.setInputMode(SS_pin, AD56X4.makeChannelMask(channels));
}

void AD56X4Class::setInputMode(int SS_pin, boolean channel_D,
                               boolean channel_C, boolean channel_B,
                               boolean channel_A) {
    AD56X4.setInputMode(SS_pin,
                        AD56X4.makeChannelMask(channel_D, channel_C,
                                               channel_B, channel_A));
}

void AD56X4Class::useInternalReference(int SS_pin, boolean yesno) {
    AD56X4.writeMessage(SS_pin, AD56X4_COMMAND_REFERENCE_ONOFF, 0,
                        (word) yesno);
}


word AD56X4Class::makeChannelMask(boolean channels[]) {
    return (word) ((byte(channels[0]) << 3) | (byte(channels[1]) << 2)
                   | (byte(channels[2]) << 1) | byte(channels[3]));
}

word AD56X4Class::makeChannelMask(boolean channel_D,
                                  boolean channel_C,
                                  boolean channel_B,
                                  boolean channel_A) {
    return (word) ((byte(channel_D) << 3) | (byte(channel_C) << 2)
                   | (byte(channel_B) << 1) | byte(channel_A));
}

extern OutputPort dec;

void AD56X4Class::writeMessage(int SS_pin, byte command,
                               byte address, word data) {
    uint8_t src[3];
    src[0] = (command & 0b00111000) | (address & 0b00000111);
    src[1] = data >> 8;
    src[2] = data & 0x00ff;
    dec.disable();
    spi_write_blocking(spi_default, src, 3);
    dec.enable();
}