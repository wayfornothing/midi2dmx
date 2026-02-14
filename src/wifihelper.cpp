#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "config.h"

// ============ CONFIGURATION WIFI ============
void wifi_init() {
    Logger::info("Démarrage WiFi Access Point...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    delay(100);

    IPAddress IP = WiFi.softAPIP();
    Logger::info("AP IP: %s\n", IP.toString().c_str());
    Logger::info("SSID: %s\n", AP_SSID);
    Logger::info("Password: %s\n", AP_PASS);
}
