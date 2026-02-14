# 🎹 MIDI to DMX Interface - ESP32 💡

Interface de conversion MIDI vers DMX512 professionnelle basée sur ESP32 avec interface Web de configuration.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-ESP32-red)

## 📋 Table des matières

- [Caractéristiques](#caractéristiques)
- [Vue d'ensemble](#vue-densemble)
- [Prérequis](#prérequis)
- [Installation rapide](#installation-rapide)
- [Utilisation](#utilisation)
- [Configuration](#configuration)
- [Documentation](#documentation)
- [Schéma électronique](#schéma-électronique)
- [FAQ](#faq)
- [Contribuer](#contribuer)
- [Licence](#licence)

---

## ✨ Caractéristiques

### Fonctionnalités principales

- ✅ **Conversion MIDI → DMX512** en temps réel
- ✅ **512 canaux DMX** supportés (1 univers complet)
- ✅ **Interface Web** de configuration intuitive
- ✅ **Point d'accès WiFi** intégré (pas besoin de routeur)
- ✅ **Sauvegarde EEPROM** de la configuration
- ✅ **Monitoring en direct** des canaux DMX
- ✅ **Modes de mapping multiples** (Note→DMX, CC→DMX, Hybride)
- ✅ **API REST** pour intégration avancée
- ✅ **Statistiques temps réel** MIDI/DMX
- ✅ **Test et débogage** intégrés

### Modes de fonctionnement

#### Mode 1 : Note → Canal DMX
Chaque note MIDI contrôle directement un canal DMX
```
Note MIDI 0 → Canal DMX 1
Note MIDI 1 → Canal DMX 2
...
Note MIDI 127 → Canal DMX 128
```

#### Mode 2 : Control Change → Canal DMX
Les CC MIDI contrôlent les canaux DMX
```
CC 0 → Canal DMX 1
CC 1 → Canal DMX 2
...
CC 127 → Canal DMX 128
```

#### Mode 3 : Hybride
Accepte à la fois Notes et Control Changes pour un contrôle maximal

### Messages MIDI supportés

- ✅ **Note On/Off** : Allumage/extinction canaux
- ✅ **Control Change** : Contrôle continu
- ✅ **Pitch Bend** : Contrôle 14-bit sur 2 canaux
- ✅ **Aftertouch** : Modulation dynamique

---

## 🎯 Vue d'ensemble

```
┌──────────────┐    MIDI     ┌─────────────────┐    DMX512    ┌──────────────┐
│   Clavier    │────────────→│  ESP32 + Web    │─────────────→│  Projecteurs │
│     MIDI     │   DIN5      │    Interface    │     XLR      │     DMX      │
└──────────────┘             └─────────────────┘              └──────────────┘
                                     ↓
                                   WiFi
                                     ↓
                             ┌───────────────┐
                             │  Navigateur   │
                             │      Web      │
                             └───────────────┘
```

### Architecture matérielle

```
[Optocoupleur 6N138] ← MIDI IN
         ↓
    [ESP32 DevKit]
         ↓
[Transceiver MAX485] → DMX OUT
```

---

## 🔧 Prérequis

### Matériel requis

| Composant | Quantité | Prix indicatif |
|-----------|----------|----------------|
| **ESP32 DevKit V1 (30 pins)** ⭐ | 1 | ~8€ |
| Optocoupleur 6N138 | 1 | ~1€ |
| MAX485 / SN75176 | 1 | ~1€ |
| Résistances (220Ω, 280Ω, 10kΩ, 120Ω) | Kit | ~2€ |
| Diode 1N4148 | 1 | ~0.20€ |
| Condensateurs 100nF | 2 | ~0.50€ |
| Connecteur DIN5 femelle | 1 | ~2€ |
| Connecteur XLR3 mâle | 1 | ~3€ |
| Boîtier plastique | 1 | ~5€ |
| **Total** | | **~23€** |

**⚠️ IMPORTANT** : Utilisez un **ESP32 DevKit V1** avec puce ESP32-WROOM-32 (30 pins).
Les modèles ESP32-S2 et ESP32-C3 ne sont **PAS compatibles** (pas de Serial2).
Voir **ESP32_COMPATIBILITY.md** pour plus de détails.

### Logiciels requis

**Option 1 - Arduino IDE** :
- Arduino IDE 2.x
- Pilotes USB ESP32 (CP210x ou CH340)
- Bibliothèques : MIDI, ArduinoJson, esp_dmx

**Option 2 - PlatformIO** (recommandé) :
- Visual Studio Code + Extension PlatformIO
- Le fichier `platformio.ini` est fourni
- Les bibliothèques s'installent automatiquement

**Commun** :
- Navigateur Web moderne (Chrome, Firefox, Safari, Edge)

---

## 🚀 Installation rapide

### Méthode 1 : Arduino IDE

```bash
# Cloner ou télécharger ce repository
git clone https://github.com/votre-repo/midi-to-dmx-esp32.git
cd midi-to-dmx-esp32

# Ouvrir midi_to_dmx_esp32.ino dans Arduino IDE
```

### Méthode 2 : PlatformIO (recommandé)

```bash
# Cloner le repository
git clone https://github.com/votre-repo/midi-to-dmx-esp32.git
cd midi-to-dmx-esp32

# Renommer le fichier pour PlatformIO
mkdir src
mv midi_to_dmx_esp32.ino src/main.cpp

# Installer et build avec PlatformIO
pio run

# Upload vers l'ESP32
pio run --target upload

# Moniteur série
pio device monitor
```

Le fichier `platformio.ini` est déjà configuré avec toutes les bibliothèques nécessaires.

### 2. Installer les bibliothèques (Arduino IDE uniquement)

⚠️ **Si vous utilisez PlatformIO, cette étape est automatique, passez à l'étape 3.**

Dans Arduino IDE :
- **Tools → Manage Libraries**
  - Installer "MIDI Library" by Francois Best
  - Installer "ArduinoJson" by Benoit Blanchon
  - Installer "esp_dmx" (manuel, voir INSTALLATION.md)

### 3. Configuration ESP32

- **Tools → Board** : ESP32 Dev Module
- **Upload Speed** : 921600
- **Flash Size** : 4MB

### 4. Compilation et upload

1. Connecter ESP32 via USB
2. Sélectionner le port COM
3. Cliquer sur Upload (→)
4. Attendre la fin du téléversement

### 5. Première connexion

1. Ouvrir les paramètres WiFi de votre appareil
2. Se connecter au réseau **MIDI-DMX-Interface**
3. Mot de passe : **dmx12345**
4. Ouvrir navigateur → http://192.168.4.1
5. L'interface Web s'affiche 🎉

---

## 💻 Utilisation

### Interface Web

L'interface Web permet de :
- **Configurer** les modes de mapping MIDI→DMX
- **Monitorer** l'activité MIDI et DMX en temps réel
- **Tester** les sorties DMX
- **Sauvegarder** la configuration en EEPROM
- **Contrôler** manuellement les canaux DMX

### Accès à l'interface

1. Se connecter au WiFi : **MIDI-DMX-Interface**
2. Navigateur → **http://192.168.4.1**

### Configuration de base

1. **Mode de mapping** : Choisir Note→DMX, CC→DMX ou Hybride
2. **Canal MIDI** : 0 pour OMNI (écoute tous les canaux), ou 1-16 pour un canal spécifique
3. **Adresse DMX de départ** : 1-512 (premier canal DMX à contrôler)
4. **Échelle vélocité** : 
   - x1 → MIDI 0-127 = DMX 0-127
   - x2 → MIDI 0-127 = DMX 0-254
5. **Notes persistantes** : 
   - Activé → Les canaux restent allumés après Note Off
   - Désactivé → Les canaux s'éteignent avec Note Off

Cliquer **Sauvegarder** pour enregistrer en EEPROM (persistant après redémarrage)

### Test rapide

Dans l'interface Web :
1. Cliquer **"Test DMX (Flash)"**
2. Tous les projecteurs doivent flasher brièvement
3. Si ça fonctionne → Installation OK ✅

---

## ⚙️ Configuration

### Modes de mapping détaillés

#### Mode 1 : Note → Canal DMX (Orgue lumineux)
Idéal pour créer un orgue lumineux ou contrôler des projecteurs individuels.

**Exemple** : Notes de piano contrôlent des PAR LED
```
C0 (Note 12)  → Canal DMX 1
C#0 (Note 13) → Canal DMX 2
D0 (Note 14)  → Canal DMX 3
...
```

**Configuration** :
- Mode : 0
- Adresse de départ : 1
- Vélocité x2 pour pleine dynamique

#### Mode 2 : CC → Canal DMX (Table de mixage)
Idéal pour contrôle précis avec faders MIDI ou séquenceur.

**Exemple** : Faders MIDI contrôlent intensité RGB
```
CC 1 → Canal DMX 1 (Rouge PAR 1)
CC 2 → Canal DMX 2 (Vert PAR 1)
CC 3 → Canal DMX 3 (Bleu PAR 1)
CC 4 → Canal DMX 4 (Rouge PAR 2)
...
```

**Configuration** :
- Mode : 1
- Utiliser contrôleur avec faders/knobs
- Parfait pour automation

#### Mode 3 : Hybride (Maximum de flexibilité)
Combine les deux modes précédents.

**Exemple** : Notes déclenchent + CC ajustent
```
Note 60 → Canal DMX 60 (ON/OFF)
CC 60   → Canal DMX 60 (Intensité fine)
```

**Configuration** :
- Mode : 2
- Permet workflows créatifs complexes

### Exemples d'utilisation réels

#### Exemple 1 : Bar/Club - 12 PAR LED RGB

**Matériel** :
- 12 × PAR LED RGB (3 canaux chacun = 36 canaux total)
- Contrôleur MIDI avec 12 faders

**Configuration** :
```
Mode: CC → Canal DMX
Canal MIDI: 0 (OMNI)
Adresse DMX: 1
Vélocité: x2

Mapping:
CC 1,2,3   → PAR 1 (R,G,B) → DMX 1,2,3
CC 4,5,6   → PAR 2 (R,G,B) → DMX 4,5,6
...
CC 34,35,36 → PAR 12 (R,G,B) → DMX 34,35,36
```

#### Exemple 2 : Concert - Orgue lumineux

**Matériel** :
- Clavier MIDI 61 touches
- 61 projecteurs monocanal

**Configuration** :
```
Mode: Note → Canal DMX
Canal MIDI: 1
Adresse DMX: 1
Vélocité: x2
Persistant: Non

Le clavier devient un orgue lumineux :
- Notes graves → Lumières bleues
- Notes médium → Lumières blanches
- Notes aigus → Lumières rouges
```

#### Exemple 3 : Studio - Séquenceur DAW

**Matériel** :
- Ableton Live / Logic Pro
- 8 × Moving heads (16 canaux chacun)

**Configuration** :
```
Mode: CC → Canal DMX
Canal MIDI: 10 (piste dédiée)
Adresse DMX: 1

Dans le DAW:
- Créer piste MIDI sur canal 10
- Dessiner automation CC 1-128
- Synchroniser parfaitement son et lumière
```

---

## 📚 Documentation

### Fichiers du projet

- **README.md** : Ce fichier (vue d'ensemble)
- **platformio.ini** : Configuration PlatformIO
- **ESP32_COMPATIBILITY.md** : Guide des modèles ESP32 compatibles ⭐
- **INSTALLATION.md** : Guide d'installation détaillé (Arduino IDE)
- **SCHEMATICS.md** : Schémas électroniques complets
- **midi_to_dmx_esp32.ino** : Code source principal

### Architecture du code

```
Setup:
├── Configuration EEPROM
├── Initialisation DMX (esp_dmx)
├── Initialisation MIDI (MIDI Library)
├── Démarrage WiFi AP
└── Lancement serveur Web

Loop:
├── Lecture messages MIDI
├── Mise à jour buffer DMX
├── Envoi trames DMX (44 Hz)
└── Gestion requêtes HTTP
```

### API REST

Base URL : `http://192.168.4.1`

#### GET /api/status
Retourne les statistiques en temps réel
```json
{
  "midiMessages": 1234,
  "dmxFrames": 56789,
  "lastMidiAge": 50,
  "mode": 0,
  "activeChannels": 12,
  "dmxData": [0, 255, 128, ...]
}
```

#### GET /api/config
Retourne la configuration actuelle
```json
{
  "mode": 0,
  "midiChannel": 0,
  "dmxStartAddr": 1,
  "velocityScale": 2,
  "persistentNotes": false
}
```

#### POST /api/config
Sauvegarde une nouvelle configuration
```json
{
  "mode": 1,
  "midiChannel": 1,
  "dmxStartAddr": 10,
  "velocityScale": 2,
  "persistentNotes": true
}
```

#### POST /api/dmx/set
Contrôle manuel d'un canal DMX
```json
{
  "channel": 1,
  "value": 255
}
```

#### POST /api/dmx/clear
Éteint tous les canaux DMX

#### POST /api/test
Lance un flash test sur tous les canaux

---

## 🔌 Schéma électronique

### Vue simplifiée

```
MIDI IN                ESP32               DMX OUT
┌─────────┐       ┌──────────┐        ┌─────────┐
│ DIN5    │──────→│  GPIO22  │        │         │
│         │  6N138│          │        │ GPIO17  │──────→│ MAX485  │──────→│ XLR3    │
│         │       │          │        │         │       │         │       │         │
└─────────┘       └──────────┘        └─────────┘
```

### Détails complets

Voir **SCHEMATICS.md** pour :
- Schéma circuit d'entrée MIDI
- Schéma circuit de sortie DMX
- PCB layout
- Liste complète des composants
- Instructions de montage

---

## 🔍 FAQ

### Questions générales

**Q : Combien de canaux DMX sont supportés ?**
R : 512 canaux (1 univers DMX complet)

**Q : Puis-je utiliser plusieurs univers DMX ?**
R : Oui, en ajoutant plusieurs MAX485 sur différents GPIO. Modification du code nécessaire.

**Q : L'interface fonctionne-t-elle sur batterie ?**
R : Oui ! ESP32 peut être alimenté par batterie 5V (powerbank USB). Consommation ~200mA.

**Q : Puis-je utiliser un autre microcontrôleur ?**
R : Oui, mais le code est optimisé pour ESP32. Arduino Mega possible mais sans WiFi.

**Q : Quelle est la latence MIDI→DMX ?**
R : < 5ms en moyenne (imperceptible)

### Problèmes fréquents

**Q : Pas de signal DMX en sortie**
R : Vérifier :
1. Câblage MAX485 (DMX+ sur pin A, DMX- sur pin B)
2. GPIO21 (EN) est bien HIGH
3. Tester avec fonction "Test DMX"

**Q : Interface Web inaccessible**
R : Vérifier :
1. Connexion au bon réseau WiFi (MIDI-DMX-Interface)
2. Essayer http://192.168.4.1 (pas https)
3. Redémarrer ESP32 (bouton RST)

**Q : MIDI reçu mais DMX ne change pas**
R : Vérifier :
1. Canal MIDI configuré (0=OMNI ou 1-16)
2. Mode de mapping approprié
3. Adresse DMX de départ correcte

**Q : WiFi instable**
R : 
1. Utiliser alimentation 5V/2A stable
2. Réduire distance ESP32 ↔ appareil connecté
3. Éviter interférences (micro-ondes, Bluetooth)

### Améliorations possibles

**Q : Comment ajouter un écran OLED ?**
R : Utiliser I2C (SDA=GPIO21, SCL=GPIO22) avec bibliothèque Adafruit SSD1306

**Q : Comment connecter au WiFi existant ?**
R : Modifier le code :
```cpp
WiFi.mode(WIFI_STA);
WiFi.begin("VotreSSID", "VotreMotDePasse");
```

**Q : Comment ajouter l'authentification Web ?**
R : Utiliser bibliothèque ESP32-WebAuthentication

---

## 🤝 Contribuer

Les contributions sont les bienvenues !

### Comment contribuer

1. Fork le projet
2. Créer une branche (`git checkout -b feature/amelioration`)
3. Commit vos changements (`git commit -am 'Ajout fonctionnalité'`)
4. Push la branche (`git push origin feature/amelioration`)
5. Créer une Pull Request

### Idées de contributions

- [ ] Support multi-univers DMX (2-4 univers)
- [ ] Authentification Web
- [ ] Connexion WiFi station (client)
- [ ] Export/Import configuration JSON
- [ ] Écran OLED avec menu
- [ ] Mode Art-Net / sACN
- [ ] Bluetooth MIDI
- [ ] MIDI Thru (sortie MIDI)

---

## 📄 Licence

Ce projet est sous licence **MIT** - voir le fichier LICENSE pour plus de détails.

### Utilisation commerciale

✅ Utilisation commerciale autorisée
✅ Modification autorisée
✅ Distribution autorisée
✅ Usage privé autorisé

⚠️ Fourni "tel quel", sans garantie

---

## 🙏 Remerciements

- **Francois Best** - MIDI Library
- **someweisguy** - ESP-DMX Library
- **Benoit Blanchon** - ArduinoJson
- **Espressif** - ESP32 Arduino Core
- **Interface-Z** - Inspiration du projet original

---

## 📞 Support

- 🐛 **Issues** : https://github.com/votre-repo/issues
- 💬 **Discussions** : https://github.com/votre-repo/discussions
- 📧 **Email** : votre-email@example.com

---

## 🌟 Changelog

### Version 1.0.0 (2024-02-14)
- ✨ Première version stable
- ✅ Interface Web complète
- ✅ 3 modes de mapping MIDI→DMX
- ✅ API REST
- ✅ Sauvegarde EEPROM
- ✅ Monitoring temps réel
- ✅ Documentation complète

---

**Fait avec ❤️ pour la communauté MIDI/DMX**

🎹 **Happy Lighting!** 💡
