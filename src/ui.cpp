#include "ui.h"
#include "midi2dmx.h"

static Midi2DMXStats _stats;

void ui_init() {
    _stats = midi2dmx_get_stats();
    pinMode(LED_STATUS_PIN, OUTPUT);
    digitalWrite(LED_STATUS_PIN, LOW);
}

void ui_tick() {
    static unsigned long lastBlink = 0;
    if (millis() - _stats.lastMidiTime < 50) {
        if (millis() - lastBlink >= 100) {
            digitalWrite(LED_STATUS_PIN, !digitalRead(LED_STATUS_PIN));
            lastBlink = millis();
        }
    }
    else {
        digitalWrite(LED_STATUS_PIN, HIGH);
    }
}