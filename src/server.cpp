
#include "server.h"
#include "config.h"

#include <ArduinoJson.h>

static WebServer _server(80);


static MappingConfig _config;
static Midi2DMX _midi2dmx;

void handleRoot()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MIDI to DMX Interface</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        h1 {
            color: white;
            text-align: center;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        .card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            margin-bottom: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }
        .card h2 {
            color: #667eea;
            margin-bottom: 20px;
            border-bottom: 3px solid #667eea;
            padding-bottom: 10px;
        }
        .stats {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 20px;
        }
        .stat-box {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
        }
        .stat-box .label {
            font-size: 0.9em;
            opacity: 0.9;
            margin-bottom: 5px;
        }
        .stat-box .value {
            font-size: 2em;
            font-weight: bold;
        }
        .form-group {
            margin-bottom: 20px;
        }
        .form-group label {
            display: block;
            margin-bottom: 8px;
            font-weight: 600;
            color: #555;
        }
        .form-group select,
        .form-group input {
            width: 100%;
            padding: 12px;
            border: 2px solid #ddd;
            border-radius: 8px;
            font-size: 1em;
            transition: border-color 0.3s;
        }
        .form-group input:focus,
        .form-group select:focus {
            outline: none;
            border-color: #667eea;
        }
        .btn {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            padding: 15px 30px;
            border-radius: 8px;
            font-size: 1em;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
            width: 100%;
            margin-top: 10px;
        }
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }
        .btn-secondary {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
        }
        .btn-success {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
        }
        .dmx-monitor {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(60px, 1fr));
            gap: 5px;
            max-height: 400px;
            overflow-y: auto;
        }
        .dmx-channel {
            background: #f0f0f0;
            padding: 10px 5px;
            border-radius: 5px;
            text-align: center;
            font-size: 0.8em;
        }
        .dmx-channel.active {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            font-weight: bold;
        }
        .activity-led {
            width: 20px;
            height: 20px;
            background: #ccc;
            border-radius: 50%;
            display: inline-block;
            margin-left: 10px;
        }
        .activity-led.active {
            background: #4ade80;
            box-shadow: 0 0 10px #4ade80;
        }
        .grid-2 {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
        }
        @media (max-width: 768px) {
            .grid-2 { grid-template-columns: 1fr; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎹 MIDI to DMX Interface 💡</h1>
        
        <div class="card">
            <h2>Statistiques <span class="activity-led" id="activityLed"></span></h2>
            <div class="stats">
                <div class="stat-box">
                    <div class="label">Messages MIDI</div>
                    <div class="value" id="midiCount">0</div>
                </div>
                <div class="stat-box">
                    <div class="label">Trames DMX</div>
                    <div class="value" id="dmxCount">0</div>
                </div>
                <div class="stat-box">
                    <div class="label">Canaux actifs</div>
                    <div class="value" id="activeChannels">0</div>
                </div>
                <div class="stat-box">
                    <div class="label">Mode actuel</div>
                    <div class="value" id="currentMode">-</div>
                </div>
            </div>
        </div>

        <div class="grid-2">
            <div class="card">
                <h2>⚙️ Configuration</h2>
                <form id="configForm">
                    <div class="form-group">
                        <label>Mode de mapping</label>
                        <select id="mode" name="mode">
                            <option value="0">Note → Canal DMX</option>
                            <option value="1">CC → Canal DMX</option>
                            <option value="2">Hybride (Note + CC)</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label>Canal MIDI (0 = OMNI)</label>
                        <input type="number" id="midiChannel" name="midiChannel" min="0" max="16" value="0">
                    </div>
                    
                    <div class="form-group">
                        <label>Adresse DMX de départ</label>
                        <input type="number" id="dmxStartAddr" name="dmxStartAddr" min="1" max="512" value="1">
                    </div>
                    
                    <div class="form-group">
                        <label>Échelle vélocité</label>
                        <select id="velocityScale" name="velocityScale">
                            <option value="1">x1 (0-127)</option>
                            <option value="2">x2 (0-254)</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label>
                            <input type="checkbox" id="persistentNotes" name="persistentNotes">
                            Notes persistantes (restent allumées)
                        </label>
                    </div>
                    
                    <button type="submit" class="btn">💾 Sauvegarder</button>
                </form>
            </div>

            <div class="card">
                <h2>🎮 Actions rapides</h2>
                <button class="btn btn-success" onclick="testDMX()">✨ Test DMX (Flash)</button>
                <button class="btn btn-secondary" onclick="clearDMX()">🧹 Effacer tous les canaux</button>
                <button class="btn" onclick="setManualChannel()">🎚️ Canal manuel</button>
            </div>
        </div>

        <div class="card">
            <h2>📊 Moniteur DMX (64 premiers canaux)</h2>
            <div class="dmx-monitor" id="dmxMonitor"></div>
        </div>
    </div>

    <script>
        // Mise à jour toutes les 500ms
        setInterval(updateStatus, 500);
        
        // Chargement config initial
        loadConfig();
        
        // Génération moniteur DMX
        generateDMXMonitor();

        function updateStatus() {
            fetch('/api/status')
                .then(r => r.json())
                .then(data => {
                    document.getElementById('midiCount').textContent = data.midiMessages;
                    document.getElementById('dmxCount').textContent = data.dmxFrames;
                    document.getElementById('activeChannels').textContent = data.activeChannels;
                    
                    const modes = ['Note→DMX', 'CC→DMX', 'Hybride'];
                    document.getElementById('currentMode').textContent = modes[data.mode] || '-';
                    
                    // LED d'activité
                    const led = document.getElementById('activityLed');
                    if (data.lastMidiAge < 100) {
                        led.classList.add('active');
                    } else {
                        led.classList.remove('active');
                    }
                    
                    updateDMXMonitor(data.dmxData);
                });
        }

        function loadConfig() {
            fetch('/api/config')
                .then(r => r.json())
                .then(config => {
                    document.getElementById('mode').value = config.mode;
                    document.getElementById('midiChannel').value = config.midiChannel;
                    document.getElementById('dmxStartAddr').value = config.dmxStartAddr;
                    document.getElementById('velocityScale').value = config.velocityScale;
                    document.getElementById('persistentNotes').checked = config.persistentNotes;
                });
        }

        document.getElementById('configForm').addEventListener('submit', (e) => {
            e.preventDefault();
            const formData = new FormData(e.target);
            const config = {
                mode: parseInt(formData.get('mode')),
                midiChannel: parseInt(formData.get('midiChannel')),
                dmxStartAddr: parseInt(formData.get('dmxStartAddr')),
                velocityScale: parseInt(formData.get('velocityScale')),
                persistentNotes: document.getElementById('persistentNotes').checked
            };
            
            fetch('/api/config', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(config)
            })
            .then(r => r.json())
            .then(data => {
                alert(data.success ? '✅ Configuration sauvegardée!' : '❌ Erreur');
            });
        });

        function generateDMXMonitor() {
            const monitor = document.getElementById('dmxMonitor');
            for (let i = 1; i <= 64; i++) {
                const div = document.createElement('div');
                div.className = 'dmx-channel';
                div.id = `ch${i}`;
                div.innerHTML = `<div>${i}</div><div>0</div>`;
                monitor.appendChild(div);
            }
        }

        function updateDMXMonitor(dmxData) {
            if (!dmxData) return;
            for (let i = 1; i <= 64; i++) {
                const ch = document.getElementById(`ch${i}`);
                const value = dmxData[i] || 0;
                ch.children[1].textContent = value;
                if (value > 0) {
                    ch.classList.add('active');
                } else {
                    ch.classList.remove('active');
                }
            }
        }

        function testDMX() {
            fetch('/api/test', {method: 'POST'})
                .then(() => alert('✨ Test DMX lancé!'));
        }

        function clearDMX() {
            fetch('/api/dmx/clear', {method: 'POST'})
                .then(() => alert('🧹 Canaux DMX effacés!'));
        }

        function setManualChannel() {
            const ch = prompt('Canal DMX (1-512):');
            const val = prompt('Valeur (0-255):');
            if (ch && val) {
                fetch('/api/dmx/set', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({channel: parseInt(ch), value: parseInt(val)})
                })
                .then(() => alert('✅ Canal ' + ch + ' = ' + val));
            }
        }
    </script>
</body>
</html>
)rawliteral";

    _server.send(200, "text/html", html);
}


void handleAPIStatus() {
    StaticJsonDocument<512> doc;

    Midi2DMXStats& stats = _midi2dmx.get_stats();
    uint8_t* dmx_data = _midi2dmx.get_dmx_data();

    doc["midiMessages"] = stats.midiMessagesReceived;
    doc["dmxFrames"] = stats.dmxFramesSent;
    doc["lastMidiAge"] = millis() - stats.lastMidiTime;
    doc["mode"] = _config.mode;

    // Compte canaux actifs
    int activeChannels = 0;
    for (int i = 1; i <= DMX_CHANNELS; i++) {
        if (dmx_data[i] > 0)
            activeChannels++;
    }
    doc["activeChannels"] = activeChannels;

    // Premier 64 canaux DMX pour le moniteur
    JsonArray dmxArray = doc.createNestedArray("dmxData");
    for (int i = 0; i <= 64; i++) {
        dmxArray.add(dmx_data[i]);
    }

    String json;
    serializeJson(doc, json);
    _server.send(200, "application/json", json);
}


void handleAPIGetConfig()
{
    StaticJsonDocument<256> doc;

    doc["mode"] = _config.mode;
    doc["midiChannel"] = _config.midiChannel;
    doc["dmxStartAddr"] = _config.dmxStartAddr;
    doc["velocityScale"] = _config.velocityScale;
    doc["persistentNotes"] = _config.persistentNotes;

    String json;
    serializeJson(doc, json);
    _server.send(200, "application/json", json);
}


void handleAPISetConfig() {
    if (_server.hasArg("plain")) {
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, _server.arg("plain"));

        if (!error) {
            _config.mode = doc["mode"];
            _config.midiChannel = doc["midiChannel"];
            _config.dmxStartAddr = doc["dmxStartAddr"];
            _config.velocityScale = doc["velocityScale"];
            _config.persistentNotes = doc["persistentNotes"];

            _config.save();

            // Reconfigure MIDI avec nouveau canal
            _midi2dmx.begin(_config.midiChannel);
            // MIDI.begin(_config.midiChannel == 0 ? MIDI_CHANNEL_OMNI : _config.midiChannel);

            _server.send(200, "application/json", "{\"success\":true}");
            Serial.println("Configuration mise à jour");
            return;
        }
    }

    _server.send(400, "application/json", "{\"success\":false}");
}


void handleAPIDMXData() {
    StaticJsonDocument<2048> doc;
    JsonArray array = doc.to<JsonArray>();
    uint8_t* dmx_data = _midi2dmx.get_dmx_data();

    for (int i = 0; i <= DMX_CHANNELS; i++) {
        array.add(dmx_data[i]);
    }

    String json;
    serializeJson(doc, json);
    _server.send(200, "application/json", json);
}


void handleAPIDMXSet() {
    if (_server.hasArg("plain")) {
        StaticJsonDocument<128> doc;
        deserializeJson(doc, _server.arg("plain"));
        uint8_t* dmx_data = _midi2dmx.get_dmx_data();

        int channel = doc["channel"];
        int value = doc["value"];

        if (channel >= 1 && channel <= DMX_CHANNELS && value >= 0 && value <= 255) {
            dmx_data[channel] = value;
            _server.send(200, "application/json", "{\"success\":true}");
            return;
        }
    }

    _server.send(400, "application/json", "{\"success\":false}");
}


void handleAPIDMXClear() {
    uint8_t* dmx_data = _midi2dmx.get_dmx_data();
    memset(dmx_data + 1, 0, DMX_CHANNELS);
    _server.send(200, "application/json", "{\"success\":true}");
    Serial.println("Tous les canaux DMX effacés");
}

void handleAPITest() {
    // Flash tous les canaux
    uint8_t* dmx_data = _midi2dmx.get_dmx_data();

    for (int i = 1; i <= DMX_CHANNELS; i++) {
        dmx_data[i] = 255;
    }
    delay(200);
    memset(dmx_data + 1, 0, DMX_CHANNELS);

    _server.send(200, "application/json", "{\"success\":true}");
    Serial.println("Test DMX effectué");
}


void server_tick() {
    _server.handleClient();
}

void server_init(Midi2DMX& midi2dmx, MappingConfig& config, uint16_t port) {
    _config = config;
    _midi2dmx = midi2dmx;
    // Page principale
    _server.on("/", HTTP_GET, handleRoot);

    // API REST
    _server.on("/api/status", HTTP_GET, handleAPIStatus);
    _server.on("/api/config", HTTP_GET, handleAPIGetConfig);
    _server.on("/api/config", HTTP_POST, handleAPISetConfig);
    _server.on("/api/dmx",   HTTP_GET, handleAPIDMXData);
    _server.on("/api/dmx/set", HTTP_POST, handleAPIDMXSet);
    _server.on("/api/dmx/clear", HTTP_POST, handleAPIDMXClear);
    _server.on("/api/test", HTTP_POST, handleAPITest);

    _server.begin();
    Serial.println("Serveur Web démarré");
}