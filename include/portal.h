#pragma once

#include <Arduino.h>

const char WIFI_PORTAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>VitalGuard — Configurar WiFi</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, sans-serif;
      background: #0A192F;
      color: #E2E8F0;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 20px;
      min-height: 100vh;
    }
    .header { text-align: center; margin: 20px 0; }
    .logo {
      width: 60px; height: 60px;
      background: #4A90E2;
      border-radius: 15px;
      display: inline-flex;
      align-items: center;
      justify-content: center;
      margin-bottom: 10px;
      font-size: 30px;
    }
    .title { font-size: 24px; font-weight: 700; color: #fff; }
    .subtitle { font-size: 14px; color: #94A3B8; }
    .card {
      width: 100%; max-width: 340px;
      background: #1E3A5F;
      border-radius: 16px;
      padding: 24px;
      margin: 10px 0;
    }
    .form-group { margin-bottom: 16px; }
    .form-group label {
      display: block;
      font-size: 12px;
      font-weight: 600;
      color: #94A3B8;
      margin-bottom: 6px;
      text-transform: uppercase;
    }
    .form-group input {
      width: 100%;
      height: 44px;
      background: #0A192F;
      border: 1px solid #2D4A6F;
      border-radius: 10px;
      padding: 0 14px;
      color: #fff;
      font-size: 14px;
    }
    .form-group input:focus {
      border-color: #4A90E2;
      outline: none;
    }
    .btn-primary {
      width: 100%;
      height: 48px;
      background: #4A90E2;
      border: none;
      border-radius: 10px;
      color: #fff;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
    }
    .btn-primary:active { transform: scale(0.98); }
    .btn-secondary {
      width: 100%;
      height: 44px;
      background: transparent;
      border: 1px solid #2D4A6F;
      border-radius: 10px;
      color: #94A3B8;
      font-size: 14px;
      cursor: pointer;
      margin-top: 10px;
    }
    .status-bar {
      padding: 10px 14px;
      background: rgba(74,144,226,0.1);
      border-radius: 10px;
      margin-bottom: 10px;
      width: 100%;
      max-width: 340px;
      text-align: center;
      font-size: 14px;
    }
    .footer {
      margin-top: 20px;
      font-size: 12px;
      color: #64748B;
      text-align: center;
    }
    .network-list {
      margin: 10px 0;
      max-height: 200px;
      overflow-y: auto;
    }
    .network-item {
      padding: 10px 12px;
      background: #0A192F;
      border-radius: 8px;
      margin-bottom: 6px;
      cursor: pointer;
      border: 1px solid transparent;
      transition: border-color 0.2s;
    }
    .network-item:hover {
      border-color: #4A90E2;
    }
    .network-item.selected {
      border-color: #4A90E2;
      background: rgba(74,144,226,0.15);
    }
    .network-name {
      font-size: 14px;
      color: #E2E8F0;
    }
    .network-signal {
      font-size: 12px;
      color: #64748B;
      float: right;
    }
    .loading {
      text-align: center;
      color: #64748B;
      padding: 10px;
    }
    .error-msg {
      background: rgba(255, 0, 0, 0.15);
      color: #FF6B6B;
      padding: 10px;
      border-radius: 8px;
      margin: 10px 0;
      display: none;
      font-size: 14px;
      text-align: center;
    }
  </style>
</head>
<body>
  <div class="header">
    <div class="logo">🛡️</div>
    <div class="title">VitalGuard</div>
    <div class="subtitle">Configuraci&oacute;n WiFi</div>
  </div>

  <div class="status-bar" id="status">📡 Escaneando redes...</div>
  <div class="error-msg" id="errorMsg">❌ Error al conectar. Verifica tus credenciales.</div>

  <form action="/wifi" method="POST" id="wifiForm">
    <div class="card">
      <div class="form-group">
        <label>Red WiFi (SSID)</label>
        <input type="text" name="s" id="ssid" placeholder="Selecciona o escribe tu red" required>
      </div>
      <div class="form-group">
        <label>Contrase&ntilde;a</label>
        <input type="password" name="p" id="pass" placeholder="Contrase&ntilde;a de la red">
      </div>
      
      <div id="networkContainer">
        <div style="font-size:12px;color:#94A3B8;margin:10px 0;">Redes disponibles:</div>
        <div class="network-list" id="networkList">
          <div class="loading">🔍 Buscando redes...</div>
        </div>
      </div>

      <button type="submit" class="btn-primary" id="btnConnect">🔗 Conectar</button>
      <button type="button" class="btn-secondary" onclick="cargarRedes()">🔄 Actualizar</button>
    </div>
  </form>

  <div class="footer">VitalGuard v1.0</div>

  <script>
    let escaneando = false;
    let intentandoConectar = false;

    function cargarRedes() {
      if (escaneando) return;
      escaneando = true;
      
      const status = document.getElementById('status');
      const list = document.getElementById('networkList');
      
      status.textContent = '📡 Escaneando redes...';
      list.innerHTML = '<div class="loading">⏳ Escaneando...</div>';

      fetch('/scan')
        .then(response => {
          if (!response.ok) {
            throw new Error('Error HTTP: ' + response.status);
          }
          return response.json();
        })
        .then(data => {
          if (data.error) {
            status.textContent = '❌ ' + data.error;
            list.innerHTML = '<div class="loading" style="color:#FF6B6B;">Error: ' + data.error + '</div>';
            escaneando = false;
            return;
          }

          if (!data.networks || data.networks.length === 0) {
            status.textContent = '📡 No se encontraron redes';
            list.innerHTML = '<div class="loading">❌ No hay redes disponibles</div>';
            escaneando = false;
            return;
          }

          const redes = data.networks.filter(net => net.ssid && net.ssid.length > 0);
          
          if (redes.length === 0) {
            status.textContent = '📡 No se encontraron redes';
            list.innerHTML = '<div class="loading">❌ No hay redes disponibles</div>';
            escaneando = false;
            return;
          }

          status.textContent = '📡 ' + redes.length + ' redes encontradas';
          list.innerHTML = '';

          redes.forEach((net) => {
            const div = document.createElement('div');
            div.className = 'network-item';
            div.onclick = function() {
              document.querySelectorAll('.network-item').forEach(el => el.classList.remove('selected'));
              this.classList.add('selected');
              document.getElementById('ssid').value = net.ssid;
              document.getElementById('pass').focus();
            };
            
            let signal = '📶';
            let strength = '';
            if (net.rssi > -50) { signal = '📶'; strength = 'Excelente'; }
            else if (net.rssi > -65) { signal = '📶'; strength = 'Buena'; }
            else if (net.rssi > -75) { signal = '📶'; strength = 'Media'; }
            else { signal = '📶'; strength = 'Débil'; }
            
            div.innerHTML = `
              <span class="network-name">${net.ssid}</span>
              <span class="network-signal">${signal} ${strength}</span>
            `;
            list.appendChild(div);
          });
          
          escaneando = false;
        })
        .catch(error => {
          console.error('Error:', error);
          status.textContent = '❌ Error al escanear';
          list.innerHTML = '<div class="loading" style="color:#FF6B6B;">❌ Error: ' + error.message + '</div>';
          escaneando = false;
        });
    }

    document.getElementById('wifiForm').addEventListener('submit', function(e) {
      const ssid = document.getElementById('ssid').value.trim();
      const pass = document.getElementById('pass').value.trim();
      const errorMsg = document.getElementById('errorMsg');
      
      if (!ssid || ssid.length < 1) {
        e.preventDefault();
        errorMsg.style.display = 'block';
        errorMsg.textContent = '❌ Selecciona o escribe el nombre de la red';
        return;
      }
    });

    window.addEventListener('load', function() {
      setTimeout(cargarRedes, 2000);
    });

    var scanInterval = setInterval(cargarRedes, 8000);
    window.addEventListener('beforeunload', function() {
      clearInterval(scanInterval);
    });
  </script>
</body>
</html>
)rawliteral";

const char SUCCESS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>VitalGuard — ¡Conectado!</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, sans-serif;
      background: #0A192F;
      color: #E2E8F0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 20px;
      min-height: 100vh;
    }
    .card {
      width: 100%; max-width: 340px;
      background: #1E3A5F;
      border-radius: 16px;
      padding: 40px 24px;
      text-align: center;
      border: 2px solid #27AE60;
      box-shadow: 0 0 40px rgba(39, 174, 96, 0.2);
    }
    .icon {
      font-size: 72px;
      margin-bottom: 20px;
      animation: bounce 1s ease-in-out;
    }
    @keyframes bounce {
      0%, 100% { transform: scale(1); }
      50% { transform: scale(1.2); }
    }
    .title {
      font-size: 28px;
      font-weight: 700;
      color: #27AE60;
      margin-bottom: 10px;
    }
    .subtitle {
      font-size: 16px;
      color: #94A3B8;
      margin-bottom: 8px;
    }
    .detail {
      font-size: 14px;
      color: #64748B;
      margin-top: 20px;
      padding-top: 20px;
      border-top: 1px solid #2D4A6F;
    }
    .status-dot {
      display: inline-block;
      width: 10px;
      height: 10px;
      background: #27AE60;
      border-radius: 50%;
      animation: pulse 2s infinite;
    }
    @keyframes pulse {
      0%, 100% { opacity: 1; }
      50% { opacity: 0.3; }
    }
    .footer {
      margin-top: 20px;
      font-size: 12px;
      color: #64748B;
      text-align: center;
    }
  </style>
</head>
<body>
  <div class="card">
    <div class="icon">✅</div>
    <div class="title">¡Conectado!</div>
    <div class="subtitle">VitalGuard se ha conectado exitosamente</div>
    <div style="margin: 15px 0;">
      <span class="status-dot"></span>
      <span style="color:#94A3B8;font-size:14px;"> Dispositivo en l&iacute;nea</span>
    </div>
    <div class="detail">
      <span style="color:#E2E8F0;">El dispositivo se reiniciar&aacute; autom&aacute;ticamente</span><br>
      <span style="color:#64748B;font-size:12px;">y se conectar&aacute; a tu red WiFi</span>
    </div>
  </div>
  <div class="footer">VitalGuard v1.0 — Tecnolog&iacute;a que cuida a quienes nos cuidaron</div>
</body>
</html>
)rawliteral";
