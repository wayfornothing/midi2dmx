#pragma once

#include <esp_dmx.h>
#include <MIDI.h>

#include "config.h"

typedef struct {
    unsigned long dmxFramesSent = 0;
    unsigned long midiMessagesReceived = 0;
    unsigned long lastMidiTime = 0;

    void update() {
        midiMessagesReceived++;
        lastMidiTime = millis();
    }

} Midi2DMXStats;

void midi2dmx_init(MappingConfig* config);
void midi2dmx_tick();
void midi2dmx_begin(uint8_t midi_channel);
Midi2DMXStats& midi2dmx_get_stats();
uint8_t* midi2dmx_get_dmx_data();
