# Guide de compatibilité ESP32 - MIDI to DMX

## 🎯 Modèle recommandé : ESP32 DevKit V1 (30 pins)

### Caractéristiques principales
- **Puce** : ESP32-WROOM-32 ou ESP32-WROOM-32D
- **Pins** : 30 GPIO disponibles
- **UARTs** : 3 (Serial0, Serial1, Serial2) ✅
- **Mémoire** : 520 KB SRAM, 4 MB Flash
- **WiFi/Bluetooth** : Oui
- **Prix** : ~6-8€

### Brochage UART pour MIDI/DMX

```
ESP32 DevKit V1 (30 pins)
+------------------+
|     USB          |
|                  |
| EN           D23 |
| VP           D22 | ← MIDI RX (Serial2 RX, remappé)
| VN           TX0 |
| D34          RX0 |
| D35          D21 | ← DMX Enable (MAX485 DE/RE)
| D32          D19 |
| D33          D18 |
| D25          D5  |
| D26          D17 | ← DMX TX (Serial2 TX, par défaut)
| D27          D16 | ← DMX RX (Serial2 RX, par défaut)
| D14          D4  |
| D12          D0  |
| D13          D2  | ← LED Status
| GND          D15 |
| VIN          GND |
| 3.3V        3.3V |
+------------------+
```

### ⚠️ Pourquoi utiliser GPIO22 pour MIDI RX au lieu de GPIO16 ?

Le code remappe Serial2 RX sur GPIO22 car :
1. **GPIO16 est utilisé pour DMX RX** (optionnel mais réservé)
2. **Serial2 peut être remappé** sur n'importe quel GPIO
3. **GPIO22 est libre** et n'a pas de fonction spéciale au boot

Dans le code :
```cpp
// Configuration Serial2 avec remap
Serial2.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);
//                                    ↑         ↑
//                                   RX22      TX désactivé
```

---

## ✅ Modèles ESP32 COMPATIBLES (avec Serial2)

### 1. **ESP32 DevKit V1** (30 pins) - ⭐ RECOMMANDÉ
- **Fabricants** : DOIT, AZ-Delivery, HiLetgo, KeeYees
- **Identifiants** : esp32dev, esp32doit-devkit-v1
- **UARTs** : Serial, Serial1, Serial2 ✅
- **Remarque** : Le modèle le plus courant et fiable

### 2. **NodeMCU-32S**
- **Fabricant** : AI-Thinker, NodeMCU
- **Identifiant** : nodemcu-32s
- **UARTs** : Serial, Serial1, Serial2 ✅
- **Remarque** : Compatible, légèrement plus large

### 3. **WEMOS LOLIN32**
- **Fabricant** : WEMOS
- **Identifiant** : lolin32
- **UARTs** : Serial, Serial1, Serial2 ✅
- **Remarque** : Batterie LiPo intégrée possible

### 4. **ESP32-WROVER**
- **Fabricant** : Espressif
- **Identifiant** : esp-wrover-kit
- **UARTs** : Serial, Serial1, Serial2 ✅
- **PSRAM** : 4-8 MB supplémentaires
- **Remarque** : Overkill pour ce projet mais fonctionne

### 5. **FireBeetle ESP32**
- **Fabricant** : DFRobot
- **Identifiant** : firebeetle32
- **UARTs** : Serial, Serial1, Serial2 ✅
- **Remarque** : Basse consommation, bon pour batterie

---

## ❌ Modèles ESP32 INCOMPATIBLES (sans Serial2)

### 1. **ESP32-S2** ❌
- **Problème** : 1 seul UART matériel disponible (Serial0)
- **UARTs** : Serial uniquement
- **Solution** : Utiliser SoftwareSerial (non recommandé pour DMX)
- **Verdict** : NE PAS UTILISER

### 2. **ESP32-C3** ❌
- **Problème** : Seulement 2 UARTs (Serial0, Serial1)
- **UARTs** : Serial, Serial1 (pas de Serial2)
- **Remarque** : RISC-V au lieu d'Xtensa
- **Verdict** : NE PAS UTILISER

### 3. **ESP32-S3** ⚠️ (à vérifier)
- **UARTs** : 2-3 selon configuration
- **Remarque** : Vérifier datasheet spécifique
- **Verdict** : Possible mais non testé

---

## 🔍 Comment identifier votre ESP32

### Méthode 1 : Marquage sur la puce

Regardez la puce métallique sur le module :
- `ESP32-WROOM-32` → ✅ Compatible
- `ESP32-WROOM-32D` → ✅ Compatible  
- `ESP32-WROVER` → ✅ Compatible
- `ESP32-S2` → ❌ Incompatible
- `ESP32-C3` → ❌ Incompatible

### Méthode 2 : Nombre de pins

- **30 pins (DevKit V1)** → ✅ Généralement compatible
- **38 pins (DevKit V4)** → ✅ Compatible
- **28 pins ou moins** → ⚠️ Vérifier le modèle

### Méthode 3 : Test dans le code

Uploadez ce sketch de test :

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESP32 UART Test ===");
  
  // Test Serial (USB)
  Serial.println("Serial (USB): OK");
  
  // Test Serial1
  #ifdef UART1
    Serial.println("Serial1: Available");
  #else
    Serial.println("Serial1: Not available");
  #endif
  
  // Test Serial2
  #ifdef UART2
    Serial.println("Serial2: Available ✅");
  #else
    Serial.println("Serial2: NOT AVAILABLE ❌");
    Serial.println("WARNING: Ce modèle n'est PAS compatible !");
  #endif
  
  // Info chip
  Serial.printf("\nChip: %s\n", ESP.getChipModel());
  Serial.printf("Revision: %d\n", ESP.getChipRevision());
  Serial.printf("Cores: %d\n", ESP.getChipCores());
  Serial.printf("Flash: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
}

void loop() {
  delay(5000);
  Serial.println("Système actif...");
}
```

Si vous voyez `Serial2: Available ✅`, votre ESP32 est compatible !

---

## 🛒 Où acheter (ESP32 DevKit V1)

### Sites recommandés

| Site | Prix | Livraison | Lien |
|------|------|-----------|------|
| **AliExpress** | 4-6€ | 2-4 semaines | Rechercher "ESP32 DevKit V1 30 pins" |
| **Amazon.fr** | 8-12€ | 1-2 jours | Rechercher "ESP32 WROOM AZ-Delivery" |
| **Banggood** | 5-7€ | 2-3 semaines | Rechercher "ESP32 Development Board" |
| **Mouser** | 10-15€ | 2-3 jours | Référence officielle Espressif |
| **Gotronic** | 9-12€ | 2-3 jours | Distributeur FR |

### Mots-clés de recherche

- "ESP32 DevKit V1"
- "ESP32 WROOM-32 Development Board"
- "ESP32 30 pins"
- "ESP32 WiFi Bluetooth Module"

### ⚠️ Vérifications avant achat

✅ **Vérifier** :
- Puce : ESP32-WROOM-32 ou ESP32-WROOM-32D
- Nombre de pins : 30 ou 38
- Port USB : Micro-USB ou USB-C (peu importe)
- Marquage : "DevKit V1" ou similaire

❌ **Éviter** :
- ESP32-S2 (1 seul UART)
- ESP32-C3 (RISC-V, 2 UARTs)
- Clones sans marquage clair
- Prix < 3€ (souvent contrefaçons)

---

## 🔧 Configuration PlatformIO par modèle

### ESP32 DevKit V1 (par défaut)
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
```

### NodeMCU-32S
```ini
[env:nodemcu]
platform = espressif32
board = nodemcu-32s
framework = arduino
```

### WEMOS LOLIN32
```ini
[env:lolin32]
platform = espressif32
board = lolin32
framework = arduino
```

---

## 📊 Tableau comparatif détaillé

| Modèle | Serial2 | Prix | WiFi | BT | Flash | PSRAM | Recommandation |
|--------|---------|------|------|-------|-------|-------|----------------|
| **ESP32 DevKit V1** | ✅ | 6-8€ | ✅ | ✅ | 4MB | Non | ⭐⭐⭐⭐⭐ |
| **NodeMCU-32S** | ✅ | 7-9€ | ✅ | ✅ | 4MB | Non | ⭐⭐⭐⭐ |
| **LOLIN32** | ✅ | 8-10€ | ✅ | ✅ | 4MB | Non | ⭐⭐⭐⭐ |
| **ESP32-WROVER** | ✅ | 12-15€ | ✅ | ✅ | 4MB | 4-8MB | ⭐⭐⭐ (overkill) |
| **ESP32-S2** | ❌ | 5-7€ | ✅ | ❌ | 4MB | Non | ❌ Incompatible |
| **ESP32-C3** | ❌ | 4-6€ | ✅ | ✅ | 4MB | Non | ❌ Incompatible |

---

## 🎓 Explication technique : Pourquoi Serial2 ?

### Les 3 UARTs de l'ESP32 classique

```
ESP32 Classic (WROOM-32)
├── UART0 (Serial)  : USB Debug (GPIO1/TX, GPIO3/RX)
├── UART1 (Serial1) : Flashage interne (généralement réservé)
└── UART2 (Serial2) : Libre ! ✅
    ├── TX par défaut : GPIO17
    └── RX par défaut : GPIO16
    (remappable sur n'importe quel GPIO)
```

### Notre utilisation

```
UART0 (Serial)  : USB Debug/Monitoring (115200 baud)
UART2 (Serial2) : MIDI IN (31250 baud, remappé GPIO22)
GPIO17          : DMX TX (via MAX485)
GPIO16          : DMX RX (via MAX485, optionnel)
```

### Pourquoi pas SoftwareSerial ?

❌ **SoftwareSerial** ne fonctionne PAS bien pour :
1. **MIDI** : Timing critique (31250 baud exact)
2. **DMX512** : 250 kbaud avec timing strict
3. **ESP32** : SoftwareSerial n'est pas fiable sur ESP32

✅ **Serial2 matériel** : Timing parfait, pas de CPU overhead

---

## 🚀 Installation PlatformIO

### 1. Installer PlatformIO

**Via VS Code** (recommandé) :
1. Installer Visual Studio Code
2. Extensions → Rechercher "PlatformIO IDE"
3. Installer l'extension

**Via CLI** :
```bash
pip install platformio
```

### 2. Créer le projet

```bash
# Méthode 1 : Nouveau projet
pio init --board esp32dev

# Méthode 2 : Utiliser le platformio.ini fourni
# Copier le fichier platformio.ini à la racine du projet
```

### 3. Structure du projet

```
midi-to-dmx-esp32/
├── platformio.ini          ← Configuration PlatformIO
├── src/
│   └── main.cpp            ← Renommer .ino en .cpp
├── include/
├── lib/
└── test/
```

### 4. Renommer le fichier principal

```bash
# Renommer .ino en .cpp (ou créer un lien symbolique)
mv midi_to_dmx_esp32.ino src/main.cpp
```

**OU** créer un wrapper `src/main.cpp` :
```cpp
#include <Arduino.h>
#include "../midi_to_dmx_esp32.ino"
```

### 5. Build et Upload

```bash
# Compiler
pio run

# Compiler et uploader
pio run --target upload

# Moniteur série
pio device monitor
```

---

## 🔧 Dépannage Serial2

### Problème : "Serial2 was not declared"

**Solution 1** : Vérifier le modèle ESP32
```cpp
#ifndef UART2
#error "Serial2 non disponible sur ce modèle !"
#endif
```

**Solution 2** : Remap manuel
```cpp
// Au lieu de Serial2.begin(31250);
// Utiliser HardwareSerial avec ID 2
HardwareSerial MidiSerial(2);
MidiSerial.begin(31250, SERIAL_8N1, 22, -1); // RX=22, TX=désactivé
```

**Solution 3** : Vérifier platformio.ini
```ini
build_flags = 
    -DARDUINO_USB_CDC_ON_BOOT=0  ; Désactive USB CDC (libère Serial)
```

---

## 📝 Résumé rapide

### ✅ À FAIRE
1. Acheter un **ESP32 DevKit V1 (30 pins)**
2. Vérifier le marquage : **ESP32-WROOM-32**
3. Utiliser le **platformio.ini** fourni
4. Tester avec le sketch de test Serial2

### ❌ À ÉVITER
1. ESP32-S2 (pas de Serial2)
2. ESP32-C3 (pas de Serial2)
3. Clones sans marquage
4. SoftwareSerial pour MIDI/DMX

---

**Recommandation finale** : **ESP32 DevKit V1 (30 pins)** avec puce ESP32-WROOM-32

Prix : ~6-8€ | Fiabilité : Excellente | Support : Excellent
