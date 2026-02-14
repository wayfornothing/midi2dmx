# Guide d'installation - MIDI to DMX Interface

## Configuration Arduino IDE

### 1. Installation de l'IDE

1. Télécharger Arduino IDE 2.x : https://www.arduino.cc/en/software
2. Installer et lancer l'IDE

### 2. Configuration ESP32

#### Ajouter le support ESP32

1. Aller dans **File → Preferences**
2. Dans "Additional Board Manager URLs", ajouter :
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Cliquer OK
4. Aller dans **Tools → Board → Boards Manager**
5. Chercher "esp32"
6. Installer **"esp32 by Espressif Systems"** (version 2.0.14 ou supérieure)

#### Sélectionner la carte

1. **Tools → Board → ESP32 Arduino**
2. Sélectionner **"ESP32 Dev Module"**

#### Configuration de compilation

```
Board: "ESP32 Dev Module"
Upload Speed: "921600"
CPU Frequency: "240MHz (WiFi/BT)"
Flash Frequency: "80MHz"
Flash Mode: "QIO"
Flash Size: "4MB (32Mb)"
Partition Scheme: "Default 4MB with spiffs"
Core Debug Level: "None"
PSRAM: "Disabled"
Port: [Sélectionner votre port COM/ttyUSB]
```

---

## Installation des bibliothèques

### Méthode 1 : Via le gestionnaire de bibliothèques (recommandé)

1. Aller dans **Tools → Manage Libraries** ou **Sketch → Include Library → Manage Libraries**
2. Installer les bibliothèques suivantes :

#### a) MIDI Library
```
Chercher: "MIDI Library"
Installer: "MIDI Library by Francois Best" (version 5.0.2+)
```

#### b) ArduinoJson
```
Chercher: "ArduinoJson"
Installer: "ArduinoJson by Benoit Blanchon" (version 6.21.0+)
```

#### c) ESP-DMX
⚠️ **Cette bibliothèque n'est PAS dans le gestionnaire, installation manuelle requise**

### Méthode 2 : Installation manuelle ESP-DMX

#### Option A : Via Git (recommandé)
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/someweisguy/esp_dmx.git
```

#### Option B : Téléchargement ZIP
1. Aller sur https://github.com/someweisguy/esp_dmx
2. Cliquer **Code → Download ZIP**
3. Dans Arduino IDE : **Sketch → Include Library → Add .ZIP Library**
4. Sélectionner le fichier téléchargé

#### Vérification installation
Redémarrer Arduino IDE et vérifier que dans **Sketch → Include Library**, vous voyez :
- MIDI Library
- ArduinoJson
- esp_dmx

---

## Compilation et téléversement

### 1. Ouvrir le code

1. Ouvrir le fichier `midi_to_dmx_esp32.ino`
2. L'IDE devrait créer automatiquement un dossier du même nom

### 2. Vérifier la compilation

1. Cliquer sur ✓ (Verify) pour compiler
2. Vérifier qu'il n'y a pas d'erreurs

### 3. Téléverser sur ESP32

1. Connecter l'ESP32 via USB
2. Sélectionner le bon port dans **Tools → Port**
3. Cliquer sur → (Upload)
4. Attendre la fin du téléversement

### 4. Moniteur série

1. Ouvrir **Tools → Serial Monitor**
2. Configurer : **115200 baud**
3. Vous devriez voir :

```
=================================
MIDI to DMX Interface - ESP32
=================================

Configuration DMX...
DMX configuré sur 512 canaux
Configuration MIDI...
MIDI configuré - Canal: OMNI
Démarrage WiFi Access Point...
AP IP: 192.168.4.1
SSID: MIDI-DMX-Interface
Password: dmx12345
Serveur Web démarré
Système prêt !
Accédez à l'interface: http://192.168.4.1
```

---

## Configuration WiFi

### 1. Connexion au point d'accès

Sur votre ordinateur/smartphone/tablette :
1. Ouvrir les paramètres WiFi
2. Chercher le réseau : **MIDI-DMX-Interface**
3. Mot de passe : **dmx12345**
4. Se connecter

### 2. Accès à l'interface Web

1. Ouvrir un navigateur web
2. Aller à l'adresse : **http://192.168.4.1**
3. L'interface devrait s'afficher

### 3. Personnaliser le WiFi (optionnel)

Dans le code, modifier les lignes :
```cpp
const char* ap_ssid = "MIDI-DMX-Interface";     // ← Votre SSID
const char* ap_password = "dmx12345";            // ← Votre mot de passe
```

---

## Test de fonctionnement

### Test 1 : WiFi et interface Web

✅ **Vérifier :**
- Connexion au WiFi réussie
- Accès à http://192.168.4.1
- Interface s'affiche correctement
- Statistiques se mettent à jour

### Test 2 : DMX sans MIDI

Dans l'interface Web :
1. Cliquer sur **"Test DMX (Flash)"**
2. Tous les projecteurs DMX doivent s'allumer brièvement
3. Si ça marche → Circuit DMX OK ✅

### Test 3 : MIDI sans DMX

1. Connecter un clavier MIDI à l'entrée DIN5
2. Jouer des notes
3. Dans le moniteur série, vous devriez voir :
   ```
   MIDI Note ON - Ch:1 Note:60 Vel:100
   ```
4. Si ça marche → Circuit MIDI OK ✅

### Test 4 : MIDI → DMX complet

1. Connecter MIDI IN et DMX OUT
2. Configuration dans interface Web :
   - Mode : "Note → Canal DMX"
   - Canal MIDI : 0 (OMNI)
   - Adresse DMX : 1
   - Échelle vélocité : x2
3. Sauvegarder
4. Jouer note MIDI 0 → Canal DMX 1 doit s'allumer
5. Jouer note MIDI 1 → Canal DMX 2 doit s'allumer
6. Si ça marche → Système complet OK ✅✅✅

---

## Dépannage installation

### Erreur "Board not found"
**Problème** : ESP32 non installé
**Solution** : Réinstaller le package ESP32 via Board Manager

### Erreur "MIDI.h not found"
**Problème** : Bibliothèque MIDI non installée
**Solution** : Installer via Library Manager

### Erreur "esp_dmx.h not found"
**Problème** : Bibliothèque ESP-DMX non installée
**Solution** : Installation manuelle (voir section ci-dessus)

### Erreur "ArduinoJson.h not found"
**Problème** : Bibliothèque ArduinoJson non installée
**Solution** : Installer via Library Manager

### Port COM non visible
**Problème** : Pilote USB manquant
**Solution Windows** : Installer pilote CP210x ou CH340
- CP210x : https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- CH340 : http://www.wch-ic.com/downloads/CH341SER_ZIP.html

**Solution Mac/Linux** : Généralement automatique, sinon installer pilote selon chipset

### Upload failed / Timeout
**Problème** : ESP32 non en mode bootload
**Solution** : 
1. Maintenir bouton **BOOT** sur ESP32
2. Cliquer **Upload**
3. Relâcher **BOOT** quand "Connecting..." apparaît

### WiFi ne démarre pas
**Problème** : SSID/password trop long ou caractères spéciaux
**Solution** : Utiliser uniquement lettres, chiffres, tirets
- SSID : Max 32 caractères
- Password : Min 8, max 63 caractères

---

## Versions alternatives du code

### Version minimaliste (sans WiFi)

Si vous voulez juste MIDI → DMX sans interface Web :

```cpp
#include <esp_dmx.h>
#include <MIDI.h>

#define DMX_TX_PIN 17
#define DMX_RX_PIN 16
#define DMX_EN_PIN 21
#define MIDI_RX_PIN 22

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
dmx_port_t dmxPort = 1;
uint8_t dmxData[513];

void setup() {
  Serial.begin(115200);
  
  // DMX
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_driver_install(dmxPort, &config, DMX_INTR_FLAGS_DEFAULT);
  dmx_set_pin(dmxPort, DMX_TX_PIN, DMX_RX_PIN, DMX_EN_PIN);
  memset(dmxData, 0, sizeof(dmxData));
  
  // MIDI
  Serial2.begin(31250, SERIAL_8N1, MIDI_RX_PIN, -1);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn([](byte ch, byte note, byte vel) {
    if (note < 512) dmxData[note + 1] = vel * 2;
  });
  MIDI.setHandleNoteOff([](byte ch, byte note, byte vel) {
    if (note < 512) dmxData[note + 1] = 0;
  });
  
  Serial.println("Ready!");
}

void loop() {
  MIDI.read();
  dmx_write(dmxPort, dmxData, 512);
  dmx_send(dmxPort, 512);
  dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);
}
```

Taille : ~2KB vs ~50KB (version complète)

---

## Exemples d'utilisation

### Exemple 1 : Contrôle 8 PAR LED RGB

Configuration :
- **PAR 1** : Canaux DMX 1-3 (R,G,B)
- **PAR 2** : Canaux DMX 4-6 (R,G,B)
- etc.

Mapping MIDI :
```
Mode: CC → Canal DMX
CC 1 → DMX 1 (PAR1 Rouge)
CC 2 → DMX 2 (PAR1 Vert)
CC 3 → DMX 3 (PAR1 Bleu)
CC 4 → DMX 4 (PAR2 Rouge)
...
```

### Exemple 2 : Orgue lumineux

Configuration :
```
Mode: Note → Canal DMX
Notes graves (C1-B1) → DMX 1-12 (Projecteurs graves)
Notes médium (C2-B2) → DMX 13-24 (Projecteurs médium)
Notes aigus (C3-B3) → DMX 25-36 (Projecteurs aigus)
```

### Exemple 3 : Séquenceur lumière

Utiliser un séquenceur MIDI (Ableton, Logic, etc.) :
1. Créer piste MIDI
2. Dessiner automation CC pour fade in/out
3. Envoyer vers interface MIDI-DMX
4. Synchronisation parfaite audio/lumière

---

## Mise à jour du firmware

### Via USB (recommandé)

1. Modifier le code selon vos besoins
2. Compiler et téléverser normalement
3. La configuration en EEPROM est préservée

### Via OTA (Over-The-Air) - À implémenter

Code à ajouter pour mise à jour WiFi :
```cpp
#include <ArduinoOTA.h>

// Dans setup():
ArduinoOTA.setHostname("MIDI-DMX");
ArduinoOTA.setPassword("admin");
ArduinoOTA.begin();

// Dans loop():
ArduinoOTA.handle();
```

Puis uploader via **Tools → Port → Network Port**

---

## Sauvegarde/Restauration configuration

### Export config via interface Web

```javascript
// À ajouter dans l'interface Web
function downloadConfig() {
  fetch('/api/config')
    .then(r => r.json())
    .then(config => {
      const blob = new Blob([JSON.stringify(config, null, 2)], 
        {type: 'application/json'});
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = 'midi-dmx-config.json';
      a.click();
    });
}
```

### Import config

```javascript
function uploadConfig(file) {
  const reader = new FileReader();
  reader.onload = (e) => {
    const config = JSON.parse(e.target.result);
    fetch('/api/config', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify(config)
    });
  };
  reader.readAsText(file);
}
```

---

## Support et documentation

### Ressources officielles
- **ESP32 Arduino Core** : https://github.com/espressif/arduino-esp32
- **MIDI Library** : https://github.com/FortySevenEffects/arduino_midi_library
- **ESP-DMX Library** : https://github.com/someweisguy/esp_dmx
- **ArduinoJson** : https://arduinojson.org/

### Forums
- Arduino Forum : https://forum.arduino.cc/
- ESP32 Reddit : https://www.reddit.com/r/esp32/
- DMX Control : https://www.dmxcontrol.de/forum/

### Exemples supplémentaires
- ESP-DMX exemples : https://github.com/someweisguy/esp_dmx/tree/main/examples
- MIDI exemples : https://github.com/FortySevenEffects/arduino_midi_library/tree/master/examples

---

**Bon développement ! 🚀**
