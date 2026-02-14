/*
 * MIDI to DMX Interface - ESP32
 * Conversion complète MIDI vers DMX512 avec interface Web
 *
 * Hardware requis:
 * - ESP32 DevKit
 * - Optocoupleur 6N138 (MIDI IN)
 * - MAX485 ou SN75176 (DMX OUT)
 * - Résistances, connecteurs DIN5 et XLR3
 */


#include "config.h"
#include "midi2dmx.h"
#include "server.h"
#include "wifihelper.h"

// void _serverHandler(ConfigWebServerCall call);




static MappingConfig _config;
static Midi2DMX _midi2dmx;


void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n=================================");
    Serial.println("MIDI to DMX Interface - ESP32");
    Serial.println("=================================\n");

    // Configuration LED
    pinMode(LED_STATUS_PIN, OUTPUT);
    digitalWrite(LED_STATUS_PIN, LOW);

    // Chargement configuration depuis EEPROM
    _config.load();

    _midi2dmx.init(_config);

    wifi_init();

    server_init(_midi2dmx, _config);

    Serial.println("Système prêt !");
    Serial.printf("Accédez à l'interface: http://%s\n", AP_SSID);

    digitalWrite(LED_STATUS_PIN, HIGH);
}

void loop() {

    _midi2dmx.tick();
    server_tick();

    // Clignotement LED sur activité MIDI
    static unsigned long lastBlink = 0;
    if (millis() - _midi2dmx.get_stats().lastMidiTime < 50) {
        if (millis() - lastBlink >= 100) {
            digitalWrite(LED_STATUS_PIN, !digitalRead(LED_STATUS_PIN));
            lastBlink = millis();
        }
    }
    else {
        digitalWrite(LED_STATUS_PIN, HIGH);
    }
}

