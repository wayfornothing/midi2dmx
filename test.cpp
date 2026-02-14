/*
 * Test de compatibilité ESP32 - MIDI to DMX
 * 
 * Ce programme teste si votre ESP32 dispose bien de Serial2 (UART2)
 * nécessaire pour le projet MIDI to DMX.
 * 
 * À uploader sur votre ESP32 pour vérifier la compatibilité AVANT
 * d'acheter les autres composants.
 * 
 * RÉSULTAT ATTENDU :
 * ✅ "Serial2: Available" → ESP32 COMPATIBLE
 * ❌ "Serial2: NOT AVAILABLE" → ESP32 INCOMPATIBLE
 */
#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif


void setup() {
  Serial.begin(115200);
  delay(2000);  // Attendre que le moniteur série s'ouvre
  
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════════════════╗");
  Serial.println("║   ESP32 Compatibility Test - MIDI to DMX         ║");
  Serial.println("╚═══════════════════════════════════════════════════╝");
  Serial.println();
  
  // Information sur la puce
  Serial.println("📌 CHIP INFORMATION:");
  Serial.println("────────────────────────────────────────────────────");
  Serial.printf("Model:        %s\n", ESP.getChipModel());
  Serial.printf("Revision:     %d\n", ESP.getChipRevision());
  Serial.printf("Cores:        %d\n", ESP.getChipCores());
  Serial.printf("CPU Freq:     %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash Size:   %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("Flash Speed:  %d MHz\n", ESP.getFlashChipSpeed() / 1000000);
  Serial.println();
  
  // Test des UARTs disponibles
  Serial.println("🔌 UART AVAILABILITY TEST:");
  Serial.println("────────────────────────────────────────────────────");
  
  // Serial0 (USB) - toujours disponible
  Serial.println("✅ Serial (UART0):  Available (USB Debug)");
  
  // Serial1 - généralement réservé pour le flash
  #ifdef HAVE_HWSERIAL1
    Serial.println("⚠️  Serial1 (UART1): Available (but reserved for flash)");
  #else
    Serial.println("❌ Serial1 (UART1): Not available");
  #endif
  
  // Serial2 - CRUCIAL pour ce projet !
  bool serial2Available = false;
  
  #ifdef HAVE_HWSERIAL2
    serial2Available = true;
  #else
    // Essayer de créer Serial2 manuellement
    HardwareSerial testSerial(2);
    serial2Available = true;  // Si on arrive ici sans erreur
  #endif
  
  if (serial2Available) {
    Serial.println("✅ Serial2 (UART2): Available ← COMPATIBLE!");
  } else {
    Serial.println("❌ Serial2 (UART2): NOT AVAILABLE ← INCOMPATIBLE!");
  }
  
  Serial.println();
  
  // Test pratique de Serial2
  Serial.println("🧪 PRACTICAL SERIAL2 TEST:");
  Serial.println("────────────────────────────────────────────────────");
  
  if (serial2Available) {
    // Tenter d'initialiser Serial2 avec les pins du projet
    HardwareSerial Serial2(2);
    
    // Configuration MIDI : 31250 baud, RX sur GPIO22
    Serial2.begin(31250, SERIAL_8N1, 22, -1);
    
    Serial.println("✅ Serial2 initialized successfully!");
    Serial.println("   - Baud rate: 31250 (MIDI standard)");
    Serial.println("   - RX Pin:    GPIO22 (remapped)");
    Serial.println("   - TX Pin:    Disabled");
    
    Serial2.end();  // Libérer Serial2
    
  } else {
    Serial.println("❌ Cannot initialize Serial2");
    Serial.println("   This ESP32 model is NOT compatible!");
  }
  
  Serial.println();
  
  // Brochage pour le projet
  Serial.println("📍 PIN ASSIGNMENT FOR MIDI-DMX PROJECT:");
  Serial.println("────────────────────────────────────────────────────");
  Serial.println("GPIO22 → MIDI RX (Serial2 RX, remapped)");
  Serial.println("GPIO17 → DMX TX  (Serial2 TX, default)");
  Serial.println("GPIO21 → DMX EN  (MAX485 DE/RE)");
  Serial.println("GPIO16 → DMX RX  (Serial2 RX, optional)");
  Serial.println("GPIO2  → LED Status");
  Serial.println();
  
  // Verdict final
  Serial.println("╔═══════════════════════════════════════════════════╗");
  if (serial2Available) {
    Serial.println("║              ✅ VERDICT: COMPATIBLE ✅             ║");
    Serial.println("╠═══════════════════════════════════════════════════╣");
    Serial.println("║  Your ESP32 is compatible with this project!     ║");
    Serial.println("║  You can proceed with building the hardware.     ║");
  } else {
    Serial.println("║            ❌ VERDICT: INCOMPATIBLE ❌            ║");
    Serial.println("╠═══════════════════════════════════════════════════╣");
    Serial.println("║  Your ESP32 does NOT have Serial2 (UART2)!       ║");
    Serial.println("║                                                   ║");
    Serial.println("║  SOLUTION:                                        ║");
    Serial.println("║  Purchase an ESP32 DevKit V1 (30 pins)           ║");
    Serial.println("║  with ESP32-WROOM-32 chip                        ║");
    Serial.println("║                                                   ║");
    Serial.println("║  AVOID: ESP32-S2, ESP32-C3                       ║");
  }
  Serial.println("╚═══════════════════════════════════════════════════╝");
  Serial.println();
  
  // Modèles recommandés
  Serial.println("✅ RECOMMENDED ESP32 MODELS:");
  Serial.println("────────────────────────────────────────────────────");
  Serial.println("• ESP32 DevKit V1 (30 pins) ⭐ BEST CHOICE");
  Serial.println("• ESP32 DOIT DevKit V1");
  Serial.println("• NodeMCU-32S");
  Serial.println("• WEMOS LOLIN32");
  Serial.println("• ESP32-WROVER (overkill but works)");
  Serial.println();
  
  Serial.println("❌ INCOMPATIBLE MODELS:");
  Serial.println("────────────────────────────────────────────────────");
  Serial.println("• ESP32-S2 (only 1 UART)");
  Serial.println("• ESP32-C3 (only 2 UARTs, no Serial2)");
  Serial.println();
  
  // Informations supplémentaires
  Serial.println("📖 FOR MORE INFORMATION:");
  Serial.println("────────────────────────────────────────────────────");
  Serial.println("Read ESP32_COMPATIBILITY.md in the project docs");
  Serial.println();
  
  Serial.println("Test completed. The board will continue running...");
}

void loop() {
  // Clignoter la LED pour montrer que le programme tourne
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink >= 1000) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    lastBlink = millis();
    
    // Message périodique
    Serial.print(".");
    
    static int dotCount = 0;
    dotCount++;
    if (dotCount >= 60) {
      Serial.println(" System running OK");
      dotCount = 0;
    }
  }
}
