
#include "midi2dmx.h"


MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_HARDWARE_SERIAL, _midi);

MappingConfig* _config;

dmx_port_t _dmx_port = 1;
uint8_t _dmx_data[DMX_CHANNELS + 1]; // +1 pour start code

Midi2DMXStats _stats;

// ============ CONFIGURATION DMX ============
void setupDMX() {
    Logger::info("Configuration DMX...");

    dmx_config_t dmxConfig = DMX_CONFIG_DEFAULT;
    dmx_driver_install(_dmx_port, &dmxConfig, nullptr, 0);
    dmx_set_pin(_dmx_port, DMX_TX_PIN, DMX_RX_PIN, DMX_EN_PIN);

    // Initialisation buffer DMX
    memset(_dmx_data, 0, sizeof(_dmx_data));
    _dmx_data[0] = 0; // Start code DMX512

    Logger::info("DMX configuré sur 512 canaux");
}

void sendDMX() {
    dmx_write(_dmx_port, _dmx_data, DMX_CHANNELS);
    dmx_send(_dmx_port /*, DMX_CHANNELS*/);
    dmx_wait_sent(_dmx_port, DMX_TIMEOUT_TICK);
    _stats.dmxFramesSent++;
}

// ============ HANDLERS _midi ============
void handleNoteOn(byte channel, byte note, byte velocity) {
    _stats.update();

    Logger::info("_midi Note ON - Ch:%d Note:%d Vel:%d\n", channel, note, velocity);

    switch (_config->mode) {
    // Note -> Canal DMX direct
        case 0:
            if (note < DMX_CHANNELS) {
                uint16_t dmxAddr = _config->dmxStartAddr + note;
                if (dmxAddr <= DMX_CHANNELS) {
                    _dmx_data[dmxAddr] = velocity * _config->velocityScale;
                }
            } break;

    // Mode Hybride: Note = adresse, Velocity = valeur
        case 2: {
            uint16_t dmxAddr = _config->dmxStartAddr + note;
            if (dmxAddr <= DMX_CHANNELS) {
                _dmx_data[dmxAddr] = velocity * _config->velocityScale;
            }
        } break;
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {

    _stats.update();
    Logger::info("_midi Note OFF - Ch:%d Note:%d\n", channel, note);

    if (!_config->persistentNotes) {
        switch (_config->mode) {
        case 0:
        case 2:
            if (note < DMX_CHANNELS) {
                uint16_t dmxAddr = _config->dmxStartAddr + note;
                if (dmxAddr <= DMX_CHANNELS) {
                    _dmx_data[dmxAddr] = 0;
                }
            }
            break;
        }
    }
}

void handleControlChange(byte channel, byte number, byte value) {
    
    _stats.update();
    Logger::info("_midi CC - Ch:%d CC:%d Val:%d\n", channel, number, value);

    switch (_config->mode) {
    case 1: // CC -> Canal DMX
        if (number < DMX_CHANNELS) {
            uint16_t dmxAddr = _config->dmxStartAddr + number;
            if (dmxAddr <= DMX_CHANNELS) {
                _dmx_data[dmxAddr] = value * _config->velocityScale;
            }
        }
        break;

    case 2: // Mode Hybride: CC aussi accepté
        if (number < DMX_CHANNELS) {
            uint16_t dmxAddr = _config->dmxStartAddr + number;
            if (dmxAddr <= DMX_CHANNELS) {
                _dmx_data[dmxAddr] = value * _config->velocityScale;
            }
        }
        break;
    }
}

void handlePitchBend(byte channel, int bend) {
    
    _stats.update();

    // Pitch bend sur canal DMX 1-2 (MSB/LSB)
    uint16_t bendValue = bend + 8192; // 0-16383
    if (_config->dmxStartAddr + 1 <= DMX_CHANNELS) {
        _dmx_data[_config->dmxStartAddr] = bendValue >> 8;       // MSB
        _dmx_data[_config->dmxStartAddr + 1] = bendValue & 0xFF; // LSB
    }
}

void handleAfterTouch(byte channel, byte note, byte pressure) {
    
    _stats.update();

    // AfterTouch modifie l'intensité de la note correspondante
    if (note < DMX_CHANNELS) {
        uint16_t dmxAddr = _config->dmxStartAddr + note;
        if (dmxAddr <= DMX_CHANNELS) {
            _dmx_data[dmxAddr] = pressure * _config->velocityScale;
        }
    }
}


// ============ CONFIGURATION _midi ============
void setupMIDI() {
    Logger::info("Configuration _midi...");

    MIDI_HARDWARE_SERIAL.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);

    midi2dmx_begin(_config->midiChannel);

    // Enregistrement des callbacks
    _midi.setHandleNoteOn(handleNoteOn);
    _midi.setHandleNoteOff(handleNoteOff);
    _midi.setHandleControlChange(handleControlChange);
    _midi.setHandlePitchBend(handlePitchBend);
    _midi.setHandleAfterTouchPoly(handleAfterTouch);

    Logger::info("_midi configuré - Canal: %s\n",
                 _config->midiChannel == 0 ? "OMNI" : String(_config->midiChannel).c_str());
}


// --- public APIs
void midi2dmx_init(MappingConfig* config) {
    _config = config;
    setupDMX();
    setupMIDI();
}


void midi2dmx_tick() {
    _midi.read();

    // Envoi DMX (44Hz refresh rate)
    static unsigned long lastDMXSend = 0;
    if (millis() - lastDMXSend >= 23) {
        // ~44 FPS
        sendDMX();
        lastDMXSend = millis();
    }
}


void midi2dmx_begin(uint8_t midi_channel) {
    _midi.begin(midi_channel == 0 ? MIDI_CHANNEL_OMNI : midi_channel);
}


uint8_t* midi2dmx_get_dmx_data() {
    return _dmx_data;
}


Midi2DMXStats& midi2dmx_get_stats() {
    return _stats;
}
