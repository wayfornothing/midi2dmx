#pragma once

#include <EEPROM.h>
#include <Arduino.h>

// ============ CONFIGURATION PINS ============
#define DMX_TX_PIN 17 // TX vers MAX485
#define DMX_RX_PIN 16 // RX depuis MAX485 (non utilisé en TX only)
#define DMX_EN_PIN 21 // Enable MAX485 (DE et RE)

#define MIDI_RX_PIN 22   // RX MIDI depuis optocoupleur
#define MIDI_HARDWARE_SERIAL Serial2

#define LED_STATUS_PIN 2 // LED embarquée ESP32


// ============ CONFIGURATION DMX ============
#define DMX_CHANNELS 512
#define DMX_UNIVERSE_SIZE 512


// ============ CONFIGURATION WIFI ============
#define AP_SSID "MIDI-DMX-Interface"
#define AP_PASS "dmx12345"


// Configuration mapping MIDI->DMX
struct MappingConfig {

    uint8_t mode;          // 0=Note->Channel, 1=CC->Channel, 2=Hybrid
    uint8_t midiChannel;   // Canal MIDI à écouter (0=OMNI)
    uint8_t dmxStartAddr;  // Adresse de départ DMX
    uint8_t velocityScale; // Multiplicateur vélocité (1-2)
    bool persistentNotes;  // Notes restent allumées après note-off


    // ============ GESTION EEPROM ============
    #define EEPROM_SIZE 64
    #define EEPROM_MAGIC 0xA5

    void save() {
        EEPROM.begin(EEPROM_SIZE);
        EEPROM.write(0, EEPROM_MAGIC);
        EEPROM.write(1, mode);
        EEPROM.write(2, midiChannel);
        EEPROM.write(3, dmxStartAddr);
        EEPROM.write(4, velocityScale);
        EEPROM.write(5, persistentNotes ? 1 : 0);
        EEPROM.commit();

        Serial.println("Configuration sauvegardée en EEPROM");
    }

    void load() {
        EEPROM.begin(EEPROM_SIZE);

        if (EEPROM.read(0) == EEPROM_MAGIC) {
            mode = EEPROM.read(1);
            midiChannel = EEPROM.read(2);
            dmxStartAddr = EEPROM.read(3);
            velocityScale = EEPROM.read(4);
            persistentNotes = EEPROM.read(5) == 1;

            Serial.println("Configuration chargée depuis EEPROM");
        }
        else {
            // Configuration par défaut
            mode = 0;
            midiChannel = 0;
            dmxStartAddr = 1;
            velocityScale = 2;
            persistentNotes = false;

            Serial.println("Configuration par défaut chargée");
        }
    }
};
