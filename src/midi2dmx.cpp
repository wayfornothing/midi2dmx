
#include "midi2dmx.h"


MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_HARDWARE_SERIAL, MIDI);

MappingConfig _config;

dmx_port_t dmxPort = 1;
uint8_t dmxData[DMX_CHANNELS + 1]; // +1 pour start code

Midi2DMXStats _stats;

// ============ CONFIGURATION DMX ============
void setupDMX() {
    Serial.println("Configuration DMX...");

    dmx_config_t dmxConfig = DMX_CONFIG_DEFAULT;
    dmx_driver_install(dmxPort, &dmxConfig, nullptr, 0);
    dmx_set_pin(dmxPort, DMX_TX_PIN, DMX_RX_PIN, DMX_EN_PIN);

    // Initialisation buffer DMX
    memset(dmxData, 0, sizeof(dmxData));
    dmxData[0] = 0; // Start code DMX512

    Serial.println("DMX configuré sur 512 canaux");
}

void sendDMX() {
    dmx_write(dmxPort, dmxData, DMX_CHANNELS);
    dmx_send(dmxPort /*, DMX_CHANNELS*/);
    dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);
    _stats.dmxFramesSent++;
}


void update_stats() {
    _stats.midiMessagesReceived++;
    _stats.lastMidiTime = millis();
}


// ============ HANDLERS MIDI ============
void handleNoteOn(byte channel, byte note, byte velocity) {
    update_stats();

    Serial.printf("MIDI Note ON - Ch:%d Note:%d Vel:%d\n", channel, note, velocity);

    switch (_config.mode) {
    // Note -> Canal DMX direct
        case 0:
            if (note < DMX_CHANNELS) {
                uint16_t dmxAddr = _config.dmxStartAddr + note;
                if (dmxAddr <= DMX_CHANNELS) {
                    dmxData[dmxAddr] = velocity * _config.velocityScale;
                }
            } break;

    // Mode Hybride: Note = adresse, Velocity = valeur
        case 2: {
            uint16_t dmxAddr = _config.dmxStartAddr + note;
            if (dmxAddr <= DMX_CHANNELS) {
                dmxData[dmxAddr] = velocity * _config.velocityScale;
            }
        } break;
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {

    update_stats();
    Serial.printf("MIDI Note OFF - Ch:%d Note:%d\n", channel, note);

    if (!_config.persistentNotes) {
        switch (_config.mode) {
        case 0:
        case 2:
            if (note < DMX_CHANNELS) {
                uint16_t dmxAddr = _config.dmxStartAddr + note;
                if (dmxAddr <= DMX_CHANNELS) {
                    dmxData[dmxAddr] = 0;
                }
            }
            break;
        }
    }
}

void handleControlChange(byte channel, byte number, byte value) {
    update_stats();

    Serial.printf("MIDI CC - Ch:%d CC:%d Val:%d\n", channel, number, value);

    switch (_config.mode) {
    case 1: // CC -> Canal DMX
        if (number < DMX_CHANNELS) {
            uint16_t dmxAddr = _config.dmxStartAddr + number;
            if (dmxAddr <= DMX_CHANNELS) {
                dmxData[dmxAddr] = value * _config.velocityScale;
            }
        }
        break;

    case 2: // Mode Hybride: CC aussi accepté
        if (number < DMX_CHANNELS) {
            uint16_t dmxAddr = _config.dmxStartAddr + number;
            if (dmxAddr <= DMX_CHANNELS) {
                dmxData[dmxAddr] = value * _config.velocityScale;
            }
        }
        break;
    }
}

void handlePitchBend(byte channel, int bend) {
    update_stats();

    // Pitch bend sur canal DMX 1-2 (MSB/LSB)
    uint16_t bendValue = bend + 8192; // 0-16383
    if (_config.dmxStartAddr + 1 <= DMX_CHANNELS) {
        dmxData[_config.dmxStartAddr] = bendValue >> 8;       // MSB
        dmxData[_config.dmxStartAddr + 1] = bendValue & 0xFF; // LSB
    }
}

void handleAfterTouch(byte channel, byte note, byte pressure) {
    update_stats();

    // AfterTouch modifie l'intensité de la note correspondante
    if (note < DMX_CHANNELS) {
        uint16_t dmxAddr = _config.dmxStartAddr + note;
        if (dmxAddr <= DMX_CHANNELS) {
            dmxData[dmxAddr] = pressure * _config.velocityScale;
        }
    }
}


// ============ CONFIGURATION MIDI ============
void setupMIDI() {
    Serial.println("Configuration MIDI...");

    MIDI_HARDWARE_SERIAL.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);

    MIDI.begin(_config.midiChannel == 0 ? MIDI_CHANNEL_OMNI : _config.midiChannel);

    // Enregistrement des callbacks
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleAfterTouchPoly(handleAfterTouch);

    Serial.printf("MIDI configuré - Canal: %s\n",
                _config.midiChannel == 0 ? "OMNI" : String(_config.midiChannel).c_str());
}
void Midi2DMX::init(MappingConfig& config) {

    _config = config;

    setupDMX();
    setupMIDI();
}


void Midi2DMX::tick() {
    // Lecture MIDI
    MIDI.read();

    // Envoi DMX (44Hz refresh rate)
    static unsigned long lastDMXSend = 0;
    if (millis() - lastDMXSend >= 23) {
        // ~44 FPS
        sendDMX();
        lastDMXSend = millis();
    }
}

void Midi2DMX::begin(uint8_t midi_channel) {
    MIDI.begin(midi_channel == 0 ? MIDI_CHANNEL_OMNI : midi_channel);
}

// unsigned long Midi2DMX::get_last_midi_time() {
//     return lastMidiTime;
// }

// unsigned long Midi2DMX::get_recv_midi_msgs() {
//     return midiMessagesReceived;
// }

// unsigned long Midi2DMX::get_sent_dmx_frames() {
//     return midiMessagesReceived;
// }

uint8_t* Midi2DMX::get_dmx_data() {
    return dmxData;
}

 Midi2DMXStats& Midi2DMX::get_stats() {
    return _stats;
}
