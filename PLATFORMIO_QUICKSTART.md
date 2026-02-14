# 🚀 Guide de démarrage rapide - PlatformIO

Ce guide vous permet de compiler et uploader le projet MIDI to DMX en quelques minutes avec PlatformIO.

---

## 📋 Prérequis

1. **Visual Studio Code** installé
2. **Extension PlatformIO** installée
3. **ESP32 DevKit V1** connecté via USB

---

## ⚡ Installation en 5 étapes

### Étape 1 : Installer VS Code et PlatformIO

#### Windows / Mac / Linux

1. **Télécharger VS Code** : https://code.visualstudio.com/
2. **Installer VS Code**
3. **Ouvrir VS Code**
4. Aller dans **Extensions** (Ctrl+Shift+X ou Cmd+Shift+X)
5. Chercher **"PlatformIO IDE"**
6. Cliquer **Install**
7. Redémarrer VS Code

### Étape 2 : Ouvrir le projet

```bash
# Ouvrir le dossier du projet dans VS Code
File → Open Folder → Sélectionner le dossier midi-to-dmx-esp32
```

**OU** en ligne de commande :
```bash
cd /chemin/vers/midi-to-dmx-esp32
code .
```

### Étape 3 : Structure du projet

Organiser les fichiers comme suit :

```
midi-to-dmx-esp32/
├── platformio.ini              ← Configuration PlatformIO
├── src/
│   └── main.cpp                ← Code principal (renommé depuis .ino)
├── include/
│   └── README                  ← Fichiers .h si nécessaire
├── lib/
│   └── README                  ← Bibliothèques locales
├── test/
│   └── README                  ← Tests unitaires
├── .gitignore
└── README.md
```

**Important** : Renommer `midi_to_dmx_esp32.ino` en `src/main.cpp`

```bash
# Sur Linux/Mac
mkdir -p src
mv midi_to_dmx_esp32.ino src/main.cpp

# Sur Windows (PowerShell)
New-Item -ItemType Directory -Force -Path src
Move-Item midi_to_dmx_esp32.ino src/main.cpp
```

**Alternative** : Créer un wrapper `src/main.cpp` :
```cpp
#include <Arduino.h>

// Inclure le fichier .ino original
#include "../midi_to_dmx_esp32.ino"
```

### Étape 4 : Connecter l'ESP32

1. Connecter l'ESP32 via USB
2. Vérifier le port détecté dans PlatformIO :
   - Cliquer sur l'icône **PlatformIO** (tête de fourmis) à gauche
   - Aller dans **Devices** pour voir le port COM

### Étape 5 : Compiler et uploader

#### Méthode 1 : Interface graphique

1. Ouvrir la **barre PlatformIO** en bas de VS Code
2. Cliquer sur **→** (icône upload) ou **✓** (icône build)

Raccourcis :
- **Build** : Ctrl+Alt+B (Windows/Linux) ou Cmd+Shift+B (Mac)
- **Upload** : Ctrl+Alt+U (Windows/Linux) ou Cmd+Shift+U (Mac)
- **Monitor** : Ctrl+Alt+S (Windows/Linux) ou Cmd+Shift+M (Mac)

#### Méthode 2 : Ligne de commande

```bash
# Compiler uniquement
pio run

# Compiler et uploader
pio run --target upload

# Moniteur série
pio device monitor

# Tout en une fois : build + upload + monitor
pio run --target upload && pio device monitor
```

---

## 🔧 Configuration du projet

Le fichier `platformio.ini` est déjà configuré :

```ini
[platformio]
default_envs = esp32dev

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600

lib_deps = 
    fortyseveneffects/MIDI Library @ ^5.0.2
    bblanchon/ArduinoJson @ ^6.21.4
    https://github.com/someweisguy/esp_dmx.git
```

### Changer le modèle d'ESP32

Si vous utilisez un modèle différent, modifiez `platformio.ini` :

```ini
# Pour NodeMCU-32S
[env:nodemcu]
board = nodemcu-32s

# Pour LOLIN32
[env:lolin32]
board = lolin32
```

Puis compiler avec :
```bash
pio run -e nodemcu
```

---

## 📊 Moniteur série

### Ouvrir le moniteur

**Interface** : Cliquer sur l'icône **prise** 🔌 dans la barre PlatformIO

**Ligne de commande** :
```bash
pio device monitor
```

**Quitter** : Ctrl+C

### Configuration moniteur

Dans `platformio.ini` :
```ini
monitor_speed = 115200
monitor_filters = 
    colorize                    ; Couleurs dans le terminal
    esp32_exception_decoder     ; Décode les stack traces
```

---

## 🐛 Dépannage

### Erreur : "Port not found"

**Solution Windows** :
1. Installer pilote CP210x : https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Ou pilote CH340 : http://www.wch-ic.com/downloads/CH341SER_ZIP.html

**Solution Mac** :
```bash
# Installer homebrew si nécessaire
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Installer pyserial
brew install pyserial
```

**Solution Linux** :
```bash
# Ajouter l'utilisateur au groupe dialout
sudo usermod -a -G dialout $USER

# Se déconnecter/reconnecter

# Vérifier les ports
ls -l /dev/ttyUSB*
```

### Erreur : "espcomm_upload_mem failed"

**Solution** :
1. Maintenir le bouton **BOOT** sur l'ESP32
2. Cliquer **Upload**
3. Relâcher **BOOT** quand "Connecting..." apparaît

### Erreur : "Library not found"

**Solution** :
```bash
# Nettoyer et réinstaller
pio lib install

# Ou forcer la mise à jour
pio pkg update
```

### Erreur : "Serial2 was not declared"

**Cause** : ESP32-S2 ou ESP32-C3 (incompatible)

**Solution** : Acheter un ESP32 DevKit V1 avec ESP32-WROOM-32

**Test** : Uploader `test_esp32_compatibility.ino` pour vérifier

---

## 📦 Commandes PlatformIO utiles

```bash
# Lister les boards disponibles
pio boards espressif32

# Informations sur le projet
pio project config

# Nettoyer le build
pio run --target clean

# Mettre à jour les bibliothèques
pio pkg update

# Rechercher une bibliothèque
pio pkg search "MIDI"

# Installer une bibliothèque spécifique
pio pkg install --library "MIDI Library"

# Lister les périphériques connectés
pio device list

# Upload avec port spécifique
pio run --target upload --upload-port /dev/ttyUSB0

# Build avec verbosité
pio run -v

# Tests unitaires
pio test
```

---

## 🎯 Workflow de développement

### 1. Développement

```bash
# Lancer le moniteur en continu
pio device monitor &

# Build et upload automatique à chaque modification
# (utiliser un outil comme nodemon ou entr)
```

### 2. Debug

Modifier `platformio.ini` :
```ini
[env:esp32dev_debug]
extends = env:esp32dev
build_type = debug
build_flags = 
    ${env:esp32dev.build_flags}
    -DCORE_DEBUG_LEVEL=5
    -DDEBUG_ESP_PORT=Serial
```

Compiler en mode debug :
```bash
pio run -e esp32dev_debug --target upload
```

### 3. Test Serial2

Avant de souder les composants, tester Serial2 :

```bash
# Uploader le test
pio run --target upload

# Voir le résultat
pio device monitor
```

Chercher dans la sortie :
```
✅ Serial2: Available ← COMPATIBLE!
```

---

## 🚀 Première utilisation complète

```bash
# 1. Cloner le projet
git clone https://github.com/votre-repo/midi-to-dmx-esp32.git
cd midi-to-dmx-esp32

# 2. Renommer le fichier principal
mkdir src
mv midi_to_dmx_esp32.ino src/main.cpp

# 3. Ouvrir dans VS Code
code .

# 4. Installer les dépendances (automatique)
# PlatformIO détecte le platformio.ini et installe tout

# 5. Connecter l'ESP32 via USB

# 6. Build, Upload et Monitor en une commande
pio run --target upload && pio device monitor
```

Résultat attendu :
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

## 🌐 Accès à l'interface Web

1. Se connecter au WiFi **"MIDI-DMX-Interface"**
2. Mot de passe : **"dmx12345"**
3. Navigateur → **http://192.168.4.1**
4. Configuration et monitoring en temps réel ! 🎉

---

## 📚 Ressources supplémentaires

- **Documentation PlatformIO** : https://docs.platformio.org/
- **ESP32 Arduino Core** : https://docs.espressif.com/projects/arduino-esp32/
- **Guide complet** : Voir README.md et INSTALLATION.md

---

## ✨ Avantages de PlatformIO vs Arduino IDE

| Fonctionnalité | PlatformIO | Arduino IDE |
|----------------|------------|-------------|
| **Gestion des bibliothèques** | ✅ Automatique | ⚠️ Manuel |
| **Multi-boards** | ✅ Facile | ⚠️ Complexe |
| **Autocomplétion** | ✅ Excellent | ⚠️ Basique |
| **Debug** | ✅ Intégré | ❌ Non |
| **Tests unitaires** | ✅ Oui | ❌ Non |
| **CI/CD** | ✅ Facile | ⚠️ Difficile |
| **Vitesse** | ✅ Rapide | ⚠️ Lent |

---

**Bon développement avec PlatformIO ! 🚀**
