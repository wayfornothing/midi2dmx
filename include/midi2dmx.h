#pragma once

// void midi2dmx_init();
// void midi2dmx_tick();
// void midi_begin(uint8_t midi_channel);
// unsigned long midi_last_midi_time();


#include <esp_dmx.h>
#include <MIDI.h>
#include "config.h"

typedef struct {
    unsigned long dmxFramesSent = 0;
    unsigned long midiMessagesReceived = 0;
    unsigned long lastMidiTime = 0;
} Midi2DMXStats;

class Midi2DMX {
public:

    

    Midi2DMX() {};

    void init(MappingConfig& config);
    void tick();
    void begin(uint8_t midi_channel);
    Midi2DMXStats& get_stats();

    uint8_t* get_dmx_data();
};