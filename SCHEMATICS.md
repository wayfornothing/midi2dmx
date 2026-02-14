# Schémas Électroniques - MIDI to DMX Interface

## Vue d'ensemble du système

```
[Clavier MIDI] --MIDI--> [Optocoupleur] --> [ESP32] --> [MAX485] --DMX512--> [Projecteurs DMX]
                                               |
                                          [WiFi AP]
                                               |
                                      [Interface Web]
```

---

## 1. Circuit d'entrée MIDI (Isolation optique)

### Composants requis
- **Optocoupleur** : 6N138 ou H11L1
- **Résistances** : 
  - R1 = 220Ω (limitation courant LED optocoupleur)
  - R2 = 220Ω (pull-up sortie)
  - R3 = 280Ω (limitation courant MIDI)
- **Diode** : 1N4148 (protection polarité)
- **Connecteur** : DIN 5 broches femelle (panneau)

### Schéma MIDI IN

```
Connecteur DIN5 (Vue arrière - soudure)
        
         4   5
       /   o   \
      2 o     o 3
        \  o  /
           1

Broche 4: +5V (optionnel, non utilisé ici)
Broche 5: MIDI Signal (data)
Broche 2: GND/Shield
Broche 1: Non connecté
Broche 3: Non connecté


Circuit:

DIN5-Pin5 ----[D1 1N4148]----[R3 280Ω]----+
                                           |
                                      +----+----+
                                      |    1    | 6N138
                            GND ------| 2     6 |------ +5V
                                      |         |
                                      | 3     5 |--[R2 220Ω]--- +3.3V
                                      |    4    |
                                      +----+----+
                                           |
                                      ESP32 GPIO22 (RX MIDI)

DIN5-Pin2 ---- GND


Explications:
- D1 protège contre inversion de polarité
- R3 limite le courant LED interne du 6N138
- R2 pull-up pour sortie du phototransistor
- Isolation galvanique complète entre MIDI et ESP32
```

### Alimentation de l'optocoupleur
```
Pin 6 (VCC): +5V depuis USB ESP32
Pin 3 (GND): GND commun
Pin 5 (Pull-up): +3.3V ESP32 (compatible logique 3.3V)
```

---

## 2. Circuit de sortie DMX512 (RS-485)

### Composants requis
- **Transceiver RS-485** : MAX485 ou SN75176BP
- **Résistances** :
  - R4 = 120Ω (terminaison DMX, optionnelle, à installer sur dernier appareil)
  - R5 = 10kΩ (pull-up/down pour stabilité ligne)
  - R6 = 10kΩ
- **Connecteur** : XLR 3 broches mâle (panneau)
- **Capacité** : C1 = 100nF (découplage VCC)

### Schéma MAX485

```
                    MAX485 / SN75176
                   +-------------+
ESP32 GPIO17 (TX)--| 1  DI    B  | 6 ---- XLR Pin 2 (DMX-)
                   |             |              |
ESP32 GPIO21 (EN)--| 2  DE    A  | 7 ---- XLR Pin 3 (DMX+)
ESP32 GPIO21 (EN)--| 3  RE       |              |
                   |         VCC | 8 ---- +5V   |
ESP32 GPIO16 (RX)--| 4  RO       |              |
                   |         GND | 5 ---- GND   |
                   +-------------+         |    |
                         |                 |    |
                        GND           [R5 10k]  |
                                           |    |
                                          GND [R6 10k]
                                                |
                                               GND

Connecteur XLR 3 broches (Vue face avant - soudure)

         1
        / \
       3   2

Pin 1: GND (Shield/Masse)
Pin 2: DMX- (Data-)
Pin 3: DMX+ (Data+)


Connexions XLR:
Pin 1 (GND) ---- GND commun
Pin 2 (DMX-) --- MAX485 Pin 6 (B)
Pin 3 (DMX+) --- MAX485 Pin 7 (A)

Terminaison (à installer sur DERNIER appareil DMX uniquement):
Entre Pin 2 et Pin 3: Résistance 120Ω [R4]

Notes importantes:
- DE et RE sont reliés ensemble (GPIO21) pour contrôle TX/RX
- Pour TX-only (émission DMX), laisser EN=HIGH en permanence
- Les résistances R5/R6 stabilisent la ligne au repos
```

### Alimentation MAX485
```
Pin 8 (VCC): +5V depuis USB ESP32
Pin 5 (GND): GND commun
C1 (100nF): Entre VCC et GND (près du MAX485)
```

---

## 3. Schéma complet ESP32

### Brochage ESP32 DevKit V1

```
                        ESP32 DevKit V1
                    +-------------------+
                    |                   |
                EN  |                   | D23
                VP  |                   | D22 ---> MIDI RX (depuis 6N138)
                VN  |                   | TX0
                D34 |                   | RX0
                D35 |                   | D21 ---> MAX485 DE/RE (Enable)
                D32 |                   | D19
                D33 |                   | D18
                D25 |                   | D5
                D26 |                   | D17 ---> MAX485 DI (TX DMX)
                D27 |                   | D16 ---> MAX485 RO (RX DMX)
                D14 |                   | D4
                D12 |                   | D0
                D13 |                   | D2  ---> LED Status (interne)
                GND |                   | D15
                VIN |                   | GND
               3.3V |                   | 3.3V
                    +-------------------+
                            USB
```

### Récapitulatif des connexions

| ESP32 Pin | Fonction      | Connecté à                    |
|-----------|---------------|-------------------------------|
| GPIO22    | MIDI RX       | 6N138 Pin 5 (via pull-up)    |
| GPIO17    | DMX TX        | MAX485 Pin 1 (DI)            |
| GPIO16    | DMX RX        | MAX485 Pin 4 (RO) [optionnel]|
| GPIO21    | DMX Enable    | MAX485 Pin 2+3 (DE/RE)       |
| GPIO2     | LED Status    | LED interne ESP32            |
| 3.3V      | Alimentation  | Pull-up optocoupleur         |
| 5V (VIN)  | Alimentation  | 6N138 Pin 6 + MAX485 Pin 8   |
| GND       | Masse         | Masse commune tous circuits  |

---

## 4. Circuit complet intégré

```
                        INTERFACE MIDI TO DMX - ESP32
                        
┌─────────────────────────────────────────────────────────────────────────┐
│                                                                         │
│  MIDI IN (DIN5)          OPTOCOUPLEUR              ESP32 DevKit        │
│                                                                         │
│   Pin5 ──[D1]──[R3]──┐                                                 │
│                       ├─1    6─── +5V              ┌─────────────┐     │
│   Pin2 ──GND──────────┤2  6N138                    │             │     │
│                       │           5─[R2]─ +3.3V ───│ 3.3V        │     │
│                       └3    4───────────────────────│ GPIO22 (RX) │     │
│                                                     │             │     │
│                                                     │ GPIO17 (TX)─┼──┐  │
│  DMX OUT (XLR3)        MAX485                      │ GPIO21 (EN)─┼─┐│  │
│                                                     │ GPIO16 (RX)─┼┐││  │
│   Pin3 ──────────── A 7    1 DI ────────────────────┘            │││││  │
│   Pin2 ──────────── B 6    4 RO ──────────────────────────────────┘││││  │
│   Pin1 ──GND────────┬ 5  2,3 DE/RE ────────────────────────────────┘│││  │
│                     │      8 VCC ── +5V ── ESP32 VIN                ││  │
│                [R5 10k]                                              ││  │
│                     │                                                ││  │
│                    GND                                               ││  │
│                     │                                                ││  │
│                [R6 10k]                                              ││  │
│                     │                                                ││  │
│                    GND                                               ││  │
│                                                                      ││  │
│  Terminaison (optionnelle, sur dernier appareil):                   ││  │
│  Entre Pin2 et Pin3: [R4 120Ω]                                      ││  │
│                                                                      ││  │
│  Alimentation:                                                       ││  │
│  USB ESP32 5V ──┬── MAX485 VCC                                      ││  │
│                 └── 6N138 VCC                                        ││  │
│                                                                      ││  │
│  GND commun partout                                                  ││  │
│                                                                      ││  │
└──────────────────────────────────────────────────────────────────────┘│  │
                                                                       ││  │
                                                                       ││  │
Connexions externes:                                                   ││  │
- MIDI IN: Depuis clavier/contrôleur MIDI                             ││  │
- DMX OUT: Vers projecteurs/équipements DMX                           ││  │
- WiFi: Access Point "MIDI-DMX-Interface"                             ││  │
- USB: Programmation + Alimentation 5V                                ││  │
                                                                       ││  │
```

---

## 5. Liste de courses complète

### Composants électroniques

| Qté | Composant           | Référence      | Notes                        |
|-----|---------------------|----------------|------------------------------|
| 1   | ESP32 DevKit V1     | ESP-WROOM-32   | 30 GPIO, WiFi/BT            |
| 1   | Optocoupleur        | 6N138 ou H11L1 | Isolation MIDI              |
| 1   | Transceiver RS-485  | MAX485 ou SN75176 | Communication DMX        |
| 1   | Diode               | 1N4148         | Protection polarité         |
| 1   | Résistance 220Ω     | 1/4W           | Pour optocoupleur LED       |
| 1   | Résistance 220Ω     | 1/4W           | Pull-up sortie              |
| 1   | Résistance 280Ω     | 1/4W           | Limitation MIDI             |
| 2   | Résistance 10kΩ     | 1/4W           | Stabilisation RS-485        |
| 1   | Résistance 120Ω     | 1/4W           | Terminaison DMX (optionnel) |
| 2   | Condensateur 100nF  | Céramique      | Découplage alimentation     |
| 1   | Connecteur DIN5     | Femelle panel  | Entrée MIDI                 |
| 1   | Connecteur XLR3     | Mâle panel     | Sortie DMX                  |
| 1   | Boîtier plastique   | ~120x80x40mm   | Montage projet              |

### Câblage
- Fil souple 0.25mm² (plusieurs couleurs)
- Câble USB Micro (programmation ESP32)
- Câble DMX XLR (pour tests)
- Câble MIDI DIN5 (pour tests)

### Outils nécessaires
- Fer à souder + étain
- Pince coupante
- Pince à dénuder
- Multimètre
- Tournevis

---

## 6. Conseils de montage

### Ordre de montage recommandé

1. **Tester l'ESP32** seul avec un programme simple (blink LED)
2. **Monter le circuit MIDI IN** sur breadboard et tester avec Serial Monitor
3. **Monter le circuit DMX OUT** sur breadboard et tester avec programme test
4. **Souder sur PCB proto** ou circuit imprimé définitif
5. **Monter dans boîtier** avec connecteurs panel

### Points critiques

⚠️ **MIDI IN** : Vérifier la polarité de la diode D1 (cathode vers optocoupleur)
⚠️ **DMX OUT** : Ne PAS croiser DMX+ et DMX- (respecter A=DMX+, B=DMX-)
⚠️ **Alimentation** : 6N138 nécessite +5V, sortie compatible 3.3V ESP32
⚠️ **Masse commune** : GND unique pour MIDI, DMX et ESP32

### Tests de validation

1. **Test MIDI** : Envoyer Note On depuis clavier → Voir dans Serial Monitor
2. **Test DMX** : Fonction test flash → Vérifier projecteur s'allume
3. **Test WiFi** : Se connecter au AP → Accéder interface web
4. **Test complet** : Note MIDI → Allumage projecteur DMX

---

## 7. Variantes et améliorations

### Ajout d'un écran OLED
```
ESP32 I2C → SSD1306 128x64
GPIO21 (SDA)
GPIO22 (SCL)
```
Affichage: statuts MIDI/DMX, config, erreurs

### Ajout de boutons physiques
```
GPIO32 → Bouton Mode
GPIO33 → Bouton Test
GPIO25 → Bouton Clear
```
Avec résistances pull-up 10kΩ

### Sortie DMX isolée (optionnel)
Ajouter optocoupleur TLP281 entre ESP32 et MAX485 pour isolation galvanique complète.

### Multi-univers DMX
Utiliser plusieurs MAX485 sur différents GPIO pour supporter plusieurs univers DMX (512 canaux × N)

---

## 8. Dépannage

| Problème | Cause possible | Solution |
|----------|----------------|----------|
| Pas de MIDI reçu | Optocoupleur mal câblé | Vérifier polarité D1, connexions |
| DMX ne fonctionne pas | MAX485 mal configuré | Vérifier DE/RE = HIGH |
| Valeurs DMX incorrectes | Mauvais câblage A/B | Inverser DMX+ et DMX- |
| ESP32 ne démarre pas | Court-circuit | Vérifier pas de pont soudure |
| WiFi instable | Alimentation faible | Utiliser alim 5V/2A minimum |

---

## Ressources additionnelles

- Datasheet 6N138: https://www.vishay.com/docs/83608/6n138.pdf
- Datasheet MAX485: https://datasheets.maximintegrated.com/en/ds/MAX1487-MAX491.pdf
- MIDI 1.0 Spec: https://www.midi.org/specifications
- DMX512 Standard: https://tsp.esta.org/tsp/documents/docs/ANSI-ESTA_E1-11_2008R2018.pdf

---

**Bon montage ! 🎹💡**
