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
#include "ui.h"

static MappingConfig _config;

void setup() {

    Logger::begin_hw();
    delay(100);

    Logger::info("=================================");
    Logger::info("MIDI to DMX Interface - ESP32");
    Logger::info("=================================");

    midi2dmx_init(&_config);
    wifi_init();
    server_init(_config);

    Logger::info("Système prêt !");
    Logger::info("Accédez à l'interface: http://%s\n", AP_SSID);

    digitalWrite(LED_STATUS_PIN, HIGH);
}

void loop() {

    midi2dmx_tick();
    server_tick();
    ui_tick();
}

