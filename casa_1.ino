#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h> 

// =================================================================
// ARCHIVO HTML COMPLETO (index.html + style.css + script.js UNIFICADO)
// =================================================================
const char* HTML_CONTENT = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Home Automation - Control Local con ESP32</title>
  <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700;900&family=Poppins:wght@300;400;600;700&display=swap" rel="stylesheet">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
  <style>
/* ==========================================
   SMART HOME AUTOMATION - ESTILOS FUTURISTAS
   (style.css integrado)
   ========================================== */

/* Variables de colores tecnológicos */
:root {
    --primary-dark: #0a0a0a;
    --secondary-dark: #1a1a2e;
    --accent-blue: #00d4ff;
    --accent-cyan: #00ffcc;
    --accent-purple: #6a00ff;
    --accent-green: #00ff88;
    --accent-red: #ff3366;
    --text-primary: #ffffff;
    --text-secondary: #b8b8b8;
    --glass-bg: rgba(255, 255, 255, 0.05);
    --glass-border: rgba(255, 255, 255, 0.1);
    --glass-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.37);
}

/* Reset y estilos base */
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: 'Poppins', sans-serif;
    background: linear-gradient(135deg, var(--primary-dark) 0%, var(--secondary-dark) 50%, #16213e 100%);
    color: var(--text-primary);
    min-height: 100vh;
    display: flex;
    flex-direction: column;
}

/* Tipografía */
h1, h2, h3 {
    font-family: 'Orbitron', sans-serif;
    color: var(--accent-blue);
    text-shadow: 0 0 5px rgba(0, 212, 255, 0.5);
}

h2 {
    font-size: 2rem;
    margin-bottom: 0.5rem;
}

.subtitle {
    color: var(--text-secondary);
    margin-bottom: 2rem;
    font-size: 1.1rem;
}

.icon-lg {
    font-size: 2.5rem;
    margin-bottom: 1rem;
    color: var(--accent-cyan);
    text-shadow: 0 0 10px rgba(0, 255, 204, 0.5);
}

.text-accent-blue { color: var(--accent-blue); }
.text-accent-green { color: var(--accent-green); }
.text-accent-cyan { color: var(--accent-cyan); }
.text-accent-purple { color: var(--accent-purple); }


/* Estructura principal */
.section {
    padding: 6rem 5%;
    text-align: center;
    min-height: 50vh;
}

/* Header y Navegación */
.header {
    background: var(--primary-dark);
    padding: 1rem 5%;
    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.5);
    position: sticky;
    top: 0;
    z-index: 1000;
}

.navbar {
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.nav-container {
    width: 100%;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.nav-logo {
    font-family: 'Orbitron', sans-serif;
    font-size: 1.5rem;
    font-weight: 700;
    color: var(--accent-green);
    display: flex;
    align-items: center;
    gap: 10px;
}

.nav-menu {
    list-style: none;
    display: flex;
    gap: 2rem;
    transition: 0.3s;
}

.nav-link {
    color: var(--text-primary);
    text-decoration: none;
    font-weight: 600;
    transition: color 0.3s, text-shadow 0.3s;
    padding: 0.5rem 0;
    position: relative;
}

.nav-link::after {
    content: '';
    position: absolute;
    width: 0;
    height: 2px;
    bottom: 0;
    left: 0;
    background-color: var(--accent-blue);
    transition: width 0.3s ease-in-out;
}

.nav-link:hover::after {
    width: 100%;
}

.nav-link:hover {
    color: var(--accent-blue);
    text-shadow: 0 0 8px rgba(0, 212, 255, 0.5);
}

.nav-toggle {
    display: none; /* Oculto en desktop */
    /* Mobile nav icon styling is defined below in media queries */
}

/* Hero Section */
.hero {
    display: flex;
    align-items: center;
    justify-content: center;
    min-height: 80vh;
    /* Usamos un placeholder genérico */
    background: url('https://placehold.co/1920x1080/1a1a2e/ffffff?text=Smart+Home+Dashboard+Background') no-repeat center center/cover;
    position: relative;
}

.hero::after {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: rgba(0, 0, 0, 0.7);
    z-index: 1;
}

.hero-content {
    z-index: 2;
    animation: fadeIn 1s ease-out;
}

.hero-title {
    font-size: 3.5rem;
    margin-bottom: 1rem;
    animation: slideInLeft 1s ease-out;
}

.hero-subtitle {
    font-size: 1.5rem;
    margin-bottom: 2rem;
    animation: slideInRight 1s ease-out;
}


/* Control and Sensor Grids */
.control-grid, .sensors-grid, .features-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    max-width: 1200px;
    margin: 0 auto;
}

/* Tarjetas (Control y Sensor) - Glassmorphism */
.control-card, .sensor-card, .feature-card {
    background: var(--glass-bg);
    border-radius: 15px;
    border: 1px solid var(--glass-border);
    padding: 2rem;
    backdrop-filter: blur(10px);
    box-shadow: var(--glass-shadow);
    transition: 0.3s;
    display: flex;
    flex-direction: column;
    align-items: center;
    text-align: center;
}

.control-card:hover, .sensor-card:hover, .feature-card:hover {
    transform: translateY(-5px);
    box-shadow: 0 12px 40px 0 rgba(0, 0, 0, 0.5);
}

/* Indicador de estado (Dot) */
.status-indicator {
    font-weight: 600;
    margin: 1rem 0;
    font-size: 1.1rem;
    display: flex;
    align-items: center;
    gap: 10px;
}

.connection-dot {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    display: inline-block;
    transition: 0.3s;
}

.connection-dot.connected {
    background-color: var(--accent-green);
    box-shadow: 0 0 10px var(--accent-green);
}

.connection-dot.disconnected {
    background-color: var(--accent-red);
    box-shadow: 0 0 10px var(--accent-red);
}

/* Estado de Dispositivos (Control/Sensores) */
.control-status {
    margin: 0.5rem 0 1rem;
    display: flex;
    align-items: center;
    gap: 8px;
}
.status-dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    transition: background-color 0.3s, box-shadow 0.3s;
}
.status-text {
    font-weight: 700;
    text-transform: uppercase;
}

/* Estados de Control */
.status-text.on, .status-dot.on {
    color: var(--accent-green);
    background-color: var(--accent-green);
    box-shadow: 0 0 8px var(--accent-green);
}
.status-text.off, .status-dot.off {
    color: var(--accent-red);
    background-color: var(--accent-red);
    box-shadow: 0 0 8px var(--accent-red);
}
.status-text.open, .status-dot.open {
    color: var(--accent-cyan);
    background-color: var(--accent-cyan);
    box-shadow: 0 0 8px var(--accent-cyan);
}
.status-text.closed, .status-dot.closed {
    color: var(--accent-red);
    background-color: var(--accent-red);
    box-shadow: 0 0 8px var(--accent-red);
}


/* Botones de Control */
.control-buttons {
    display: flex;
    gap: 1rem;
    margin-top: 1.5rem;
    width: 100%;
    justify-content: center;
}

.btn {
    flex-grow: 1;
    padding: 0.75rem 1rem;
    border: none;
    border-radius: 10px;
    font-weight: 700;
    cursor: pointer;
    transition: 0.3s, transform 0.1s;
    font-family: 'Poppins', sans-serif;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
}

.btn-on {
    background-color: var(--accent-green);
    color: var(--primary-dark);
}

.btn-off {
    background-color: var(--accent-red);
    color: var(--text-primary);
    border: 2px solid var(--accent-red);
    background-color: rgba(255, 51, 102, 0.1);
}

.btn-on:hover {
    background-color: #00e676;
    transform: translateY(-2px);
    box-shadow: 0 0 15px #00e676;
}

.btn-off:hover {
    background-color: var(--accent-red);
    color: var(--text-primary);
    transform: translateY(-2px);
    box-shadow: 0 0 15px var(--accent-red);
}

.btn:active {
    transform: scale(0.98);
}


/* Sensores Específicos */

/* Barra de progreso (LDR) */
.progress-bar-container {
    width: 100%;
    height: 15px;
    background-color: rgba(255, 255, 255, 0.1);
    border-radius: 10px;
    margin: 1rem 0;
    overflow: hidden;
}

.progress-bar {
    height: 100%;
    width: 0; /* Controlado por JS */
    background-color: var(--accent-blue);
    border-radius: 10px;
    transition: width 0.5s ease;
    box-shadow: 0 0 5px var(--accent-blue);
}

.sensor-value .value-lg {
    font-family: 'Orbitron', sans-serif;
    font-size: 1.8rem;
    font-weight: 700;
    color: var(--accent-cyan);
}
.sensor-value .unit-text {
    color: var(--text-secondary);
}

/* Estados de Sensor Lluvia */
.sensor-status .status-dot.dry {
    background-color: var(--accent-green);
    box-shadow: 0 0 8px var(--accent-green);
}
.sensor-status .status-dot.wet {
    background-color: var(--accent-blue);
    box-shadow: 0 0 8px var(--accent-blue);
}

.text-secondary {
    color: var(--text-secondary);
    font-size: 0.9rem;
}

/* Footer */
.footer {
    background-color: var(--primary-dark);
    padding: 2rem 5%;
    margin-top: auto;
    border-top: 1px solid var(--glass-border);
}

.footer-content {
    display: flex;
    justify-content: space-between;
    align-items: center;
    max-width: 1200px;
    margin: 0 auto;
}

.social-links a {
    color: var(--text-secondary);
    font-size: 1.5rem;
    margin-left: 1rem;
    transition: color 0.3s;
}

.social-links a:hover {
    color: var(--accent-blue);
}


/* Notificaciones Toast */
.notification-toast {
    position: fixed;
    bottom: 20px;
    right: 20px;
    background-color: rgba(4, 43, 64, 0.9);
    color: white;
    padding: 15px 20px;
    border-radius: 10px;
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.5);
    opacity: 0;
    visibility: hidden;
    transform: translateY(20px);
    transition: opacity 0.3s, transform 0.3s, visibility 0.3s;
    display: flex;
    align-items: center;
    gap: 10px;
    z-index: 2000;
}

.notification-toast.active {
    opacity: 1;
    visibility: visible;
    transform: translateY(0);
}

.notification-toast.success {
    background-color: var(--accent-green);
    color: var(--primary-dark);
}
.notification-toast.error {
    background-color: var(--accent-red);
}
.notification-toast.info {
    background-color: var(--accent-blue);
}

/* MEDIA QUERIES (Responsiveness) */

@media (max-width: 768px) {
    /* Navegación Móvil */
    .nav-menu {
        position: fixed;
        top: 65px;
        left: -100%;
        width: 100%;
        height: 100%;
        background: var(--secondary-dark);
        flex-direction: column;
        align-items: center;
        padding: 2rem;
        box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.5);
        border-top: 1px solid var(--glass-border);
    }

    .nav-menu.active {
        left: 0;
    }

    .nav-toggle {
        display: flex;
        flex-direction: column;
        cursor: pointer;
        gap: 4px;
    }

    .nav-toggle span {
        width: 25px;
        height: 3px;
        background: var(--text-primary);
        transition: 0.3s;
    }

    .hero-title {
        font-size: 2.5rem;
    }

    .control-grid,
    .sensors-grid,
    .features-grid {
        grid-template-columns: 1fr;
    }

    .control-buttons {
        flex-direction: column;
    }

    .footer-content {
        flex-direction: column;
        text-align: center;
    }
}

/* Animaciones */
@keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }
@keyframes slideInLeft { from { opacity: 0; transform: translateX(-50px); } to { opacity: 1; transform: translateX(0); } }
@keyframes slideInRight { from { opacity: 0; transform: translateX(50px); } to { opacity: 1; transform: translateX(0); } }

  </style>
</head>
<body>
  <header class="header">
    <nav class="navbar">
      <div class="nav-container">
        <div class="nav-logo">
          <i class="fas fa-home"></i>
          <span>SmartHome</span>
        </div>
        <div class="nav-toggle">
            <span></span>
            <span></span>
            <span></span>
        </div>
        <ul class="nav-menu">
          <li><a href="#inicio" class="nav-link">Inicio</a></li>
          <li><a href="#control" class="nav-link">Control</a></li>
          <li><a href="#sensores" class="nav-link">Sensores</a></li>
          <li><a href="#estado" class="nav-link">Estado</a></li>
        </ul>
      </div>
    </nav>
  </header>

  <main class="main-content">
    
    <section id="inicio" class="hero section">
        <div class="hero-content">
            <h1 class="hero-title">Automatización Inteligente del Hogar</h1>
            <p class="hero-subtitle">Controla y monitoriza tu casa en tiempo real con ESP32 y Web Server Local.</p>
            <div class="status-indicator">
                <i id="connectionDot" class="fas fa-circle connection-dot disconnected"></i>
                <span id="connectionStatus">Cargando...</span>
            </div>
            <p class="last-update">Última actualización: <span id="lastUpdate">N/A</span></p>
        </div>
    </section>

    <section id="control" class="section">
     
        <h2 class="section-title">Control de Dispositivos</h2>
        <div class="control-grid">
            
            <div class="control-card animated-item" data-animation="slideInLeft">
                <div class="card-icon"><i class="fas fa-lightbulb"></i></div>
                <h3 class="card-title">Iluminación Principal</h3>
   
                 <div class="control-status">
                    <span id="lightStatus" class="status-text off">OFF</span>
                    <i id="lightStatusDot" class="fas fa-circle status-dot off"></i>
                </div>
                <div class="control-buttons">
     
                    <button class="btn btn-on" id="lightOnBtn"><i class="fas fa-power-off"></i> Encender</button>
                    <button class="btn btn-off" id="lightOffBtn"><i class="fas fa-times-circle"></i> Apagar</button>
                </div>
            </div>

            <div class="control-card animated-item" data-animation="slideInRight">
                <div class="card-icon"><i class="fas fa-door-open"></i></div>
                <h3 class="card-title">Puerta de Acceso</h3>
                <div class="control-status">
                    <span id="doorStatus" class="status-text closed">Cerrada</span>
              
                    <i id="doorStatusDot" class="fas fa-circle status-dot closed"></i>
                </div>
                <div class="control-buttons">
                    <button class="btn btn-on" id="doorOpenBtn"><i class="fas fa-lock-open"></i> Abrir</button>
                    <button class="btn btn-off" id="doorCloseBtn"><i class="fas fa-lock"></i> Cerrar</button>
    
                </div>
            </div>
            
        </div>
    </section>

    <section id="sensores" class="section">
        <h2 class="section-title">Monitorización de Sensores</h2>
        <div class="sensors-grid">
            
      
              <div class="sensor-card animated-item" data-animation="slideInLeft">
                <div class="card-icon"><i class="fas fa-sun"></i></div>
                <h3 class="card-title">Intensidad Lumínica (LDR)</h3>
                <div class="sensor-value">
                  
                    <span id="ldr-value" class="value-lg">0</span>
                    <span class="unit-text">/ 4095</span>
                </div>
                <div class="progress-bar-container">
                    <div class="progress-bar" id="ldr-bar" style="width: 0%;"></div>
                </div>
 
                <div class="sensor-status" id="ldr-status">
                    <span class="status-text">Oscuro</span>
                    <i class="fas fa-circle status-dot"></i>
                </div>
            </div>

           
              <div class="sensor-card animated-item" data-animation="slideInRight">
                <div class="card-icon" id="rainIcon"><i class="fas fa-cloud"></i></div>
                <h3 class="card-title">Detección de Lluvia</h3>
                <div class="sensor-value">
                    <span id="lluvia-value" class="value-lg">Seco</span>
                </div>
                <p class="text-secondary mt-2">Valor RAW (0-4095): <span id="lluvia-raw-value" class="text-accent-cyan">N/A</span></p>
                <div class="sensor-status" id="lluvia-status">
   
                  <span class="status-text">Seco</span>
                    <i class="fas fa-circle status-dot dry"></i>
                </div>
            </div>
            
        </div>
    </section>
    
  
      <section id="estado" class="section">
        <h2 class="section-title">Registro de Eventos y Estado</h2>
        <div class="features-grid">
            <div class="feature-card animated-item" data-animation="slideInLeft">
                <i class="fas fa-chart-line"></i>
                <h4>Historial de Actividad</h4>
          
                <p>Visualiza gráficas de uso y registros de todos los eventos del hogar.</p>
            </div>
            <div class="feature-card animated-item" data-animation="slideInRight">
                <i class="fas fa-bell"></i>
                <h4>Notificaciones Inteligentes</h4>
                <p>Alertas instantáneas sobre cambios de estado y eventos críticos (ej. Detección de lluvia).</p>
            </div>
        </div>
    </section>

  </main>

  <div id="notification-toast" class="notification-toast">
      <span id="notification-icon"></span>
      <p id="notification-message"></p>
  </div>

  <footer class="footer">
    <div class="footer-content">
        <p>&copy; 2024 SmartHome Automation. Desarrollado con ESP32 y Web Server.</p>
        <div class="social-links">
            <a href="#"><i class="fab fa-github"></i></a>
            <a href="#"><i class="fas fa-microchip"></i></a>
        </div>
    </div>
  </footer>

  <script>
    // URL base para el ESP32 (servidor local)
    const ESP32_URL = "";
    // ==========================================
    // NOTIFICACIONES
    // ==========================================
    function showNotification(message, type = 'info') {
        const toast = document.getElementById('notification-toast');
        const messageEl = document.getElementById('notification-message');
        const iconEl = document.getElementById('notification-icon');

        messageEl.textContent = message;
        toast.className = 'notification-toast active ' + type;
        let iconClass = '';
        if (type === 'success') {
            iconClass = 'fas fa-check-circle';
        } else if (type === 'error') {
            iconClass = 'fas fa-exclamation-triangle';
        } else {
            iconClass = 'fas fa-info-circle';
        }
        iconEl.className = iconClass;
        setTimeout(() => {
            toast.classList.remove('active');
        }, 3000);
    }

    // ==========================================
    // API Y ESTADO DEL DISPOSITIVO
    // ==========================================

    /**
     * Envía un comando de control al ESP32.
     * @param {string} dispositivo - 'luz' o 'puerta'.
     * @param {string} estado - 'ON', 'OFF', 'OPEN', 'CLOSE'.
     */
    const controlarDispositivo = async (dispositivo, estado) => {
      try {
        const response = await fetch(ESP32_URL + '/control', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ dispositivo: dispositivo, estado: estado })
      
        });

        if (!response.ok) {
          // Intentar leer el mensaje de error del ESP32 si está disponible
          const errorText = await response.text();
          throw new Error(`Error HTTP: ${response.status}. Mensaje: ${errorText}`);
        }

        const resultText = await response.text();
        showNotification(`${resultText}`, 'success');
        // Tras el control, forzar una actualización de estado
        await fetchStates();
      } catch (error) {
        console.error("Error al enviar comando:", error);
        showNotification(`Error: No se pudo controlar ${dispositivo}.`, 'error');
      }
    }


    /**
     * Obtiene y actualiza el estado de todos los dispositivos y sensores desde el ESP32.
     */
    const fetchStates = async () => {
      const statusText = document.getElementById("connectionStatus");
      const statusDot = document.getElementById("connectionDot");
      const lastUpdateText = document.getElementById("lastUpdate");

      try {
        statusText.textContent = "Conectando...";
        statusDot.className = "fas fa-circle connection-dot disconnected";

        const response = await fetch(ESP32_URL + '/status');
        if (!response.ok) {
          throw new Error(`Error HTTP: ${response.status}`);
        }

        const data = await response.json();
        updateUI(data);
        // Actualizar estado de conexión exitoso
        statusText.textContent = "Conectado";
        statusDot.className = "fas fa-circle connection-dot connected";
        lastUpdateText.textContent = new Date().toLocaleTimeString('es-ES', { hour: '2-digit', minute: '2-digit', second: '2-digit' });
      } catch (error) {
        console.error("Error al obtener estados:", error);
        statusText.textContent = "Desconectado";
        statusDot.className = "fas fa-circle connection-dot disconnected";
        lastUpdateText.textContent = "Fallo de conexión";
      }
    }
    
    /**
     * Actualiza la interfaz de usuario con los datos recibidos.
     * @param {object} data - Datos de estado del ESP32.
     */
    function updateUI(data) {
        // --- LDR (Intensidad Lumínica) ---
        const valorLDR = data.ldr || 0;
        const maxLDR = 4095; // ESP32 ADC Max Value (para 12-bit)
        
        document.getElementById("ldr-value").textContent = valorLDR;
        const percentage = Math.min(100, (valorLDR / maxLDR) * 100);
        document.getElementById("ldr-bar").style.width = percentage + "%";
        
        const ldrStatusEl = document.getElementById("ldr-status").querySelector(".status-text");
        const ldrDotEl = document.getElementById("ldr-status").querySelector(".status-dot");
        
        // La UI usa una lógica de 3 estados para LDR
        if (valorLDR > 3000) {
            ldrStatusEl.textContent = "Muy Luminoso";
            ldrDotEl.style.backgroundColor = 'var(--accent-blue)';
        } else if (valorLDR > 1500) {
            ldrStatusEl.textContent = "Luminoso";
            ldrDotEl.style.backgroundColor = 'var(--accent-cyan)';
        } else {
            ldrStatusEl.textContent = "Oscuro";
            ldrDotEl.style.backgroundColor = 'var(--accent-purple)';
        }

        // --- Lluvia ---
        const estadoLluvia = data.lluvia || 'Seco';
        const rawLluvia = data.lluvia_raw !== undefined ? data.lluvia_raw : 'N/A';
        // Nuevo: valor RAW
        
        const lluviaTextEl = document.getElementById("lluvia-value");
        const lluviaRawEl = document.getElementById("lluvia-raw-value"); // Nuevo
        const lluviaStatusEl = document.getElementById("lluvia-status").querySelector(".status-text");
        const lluviaDotEl = document.getElementById("lluvia-status").querySelector(".status-dot");
        const rainIconEl = document.getElementById("rainIcon");
        
        lluviaRawEl.textContent = rawLluvia; // Mostrar valor RAW

        if (estadoLluvia === 'Húmedo') {
          lluviaTextEl.textContent = "¡LLUVIA!";
          lluviaStatusEl.textContent = "Lluvia Detectada (Ropa Guardada)";
          lluviaDotEl.classList.remove('dry');
          lluviaDotEl.classList.add('wet');
          rainIconEl.innerHTML = '<i class="fas fa-cloud-showers-heavy text-accent-blue"></i>';
        } else {
          lluviaTextEl.textContent = "Seco";
          lluviaStatusEl.textContent = "Seco (Ropa Extendida)";
          lluviaDotEl.classList.remove('wet');
          lluviaDotEl.classList.add('dry');
          rainIconEl.innerHTML = '<i class="fas fa-cloud text-accent-cyan"></i>'; 
        }

        // --- Control de Luz (Relé) ---
        const estadoLuz = data.luz_estado || 'OFF';
        const lightStatus = document.getElementById('lightStatus');
        const lightStatusDot = document.getElementById('lightStatusDot');

        if (estadoLuz === 'ON') {
            lightStatus.textContent = "ON";
            lightStatus.classList.replace('off', 'on');
            lightStatusDot.classList.replace('off', 'on');
        } else {
            lightStatus.textContent = "OFF";
            lightStatus.classList.replace('on', 'off');
            lightStatusDot.classList.replace('on', 'off');
        }

        // --- Control de Puerta (Servo) ---
        const estadoPuerta = data.puerta_estado || 'CLOSED';
        const doorStatus = document.getElementById('doorStatus');
        const doorStatusDot = document.getElementById('doorStatusDot');

        if (estadoPuerta === 'OPEN') {
            doorStatus.textContent = "Abierta";
            doorStatus.classList.replace('closed', 'open');
            doorStatusDot.classList.replace('closed', 'open');
        } else {
            doorStatus.textContent = "Cerrada";
            doorStatus.classList.replace('open', 'closed');
            doorStatusDot.classList.replace('open', 'closed');
        }
    }


    // ==========================================
    // INICIALIZACIÓN Y EVENT LISTENERS
    // ==========================================
    
    function setupControlListeners() {
        document.getElementById('lightOnBtn').addEventListener('click', () => controlarDispositivo('luz', 'ON'));
        document.getElementById('lightOffBtn').addEventListener('click', () => controlarDispositivo('luz', 'OFF'));
        document.getElementById('doorOpenBtn').addEventListener('click', () => controlarDispositivo('puerta', 'OPEN'));
        document.getElementById('doorCloseBtn').addEventListener('click', () => controlarDispositivo('puerta', 'CLOSE'));
    }

    function initNavigation() {
        const navToggle = document.querySelector('.nav-toggle');
        const navMenu = document.querySelector('.nav-menu');
        
        if (navToggle && navMenu) {
            navToggle.addEventListener('click', () => {
                navMenu.classList.toggle('active');
                // Alternar la clase en el toggle para animación del ícono (si aplica)
                navToggle.classList.toggle('active');
            });
            // Cerrar menú al hacer clic en un enlace (en móvil)
            document.querySelectorAll('.nav-link').forEach(link => {
                link.addEventListener('click', () => {
                    navMenu.classList.remove('active');
                    navToggle.classList.remove('active');
                });
 
            });
        }
    }

    // Smooth scrolling para enlaces internos
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            if (target) {
                target.scrollIntoView({
             
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    // Función de inicialización principal
    document.addEventListener('DOMContentLoaded', () => {
        initNavigation();
        setupControlListeners();
        // Carga el estado inicial y programa actualizaciones periódicas
        fetchStates(); 
        // Actualizar el estado cada 3 segundos (suficiente para la mayoría de las apps IoT)
        setInterval(fetchStates, 3000); 
    });
    // Manejo de errores global (mejor práctica)
    window.addEventListener('error', function(e) {
        console.error('Error global en JS:', e.error);
        showNotification('Error interno en la interfaz', 'error');
    });
</script>
</body>
</html>
)rawliteral";

// --- Configuración de Conexión Wi-Fi (MODO CLIENTE STA) ---
const char *ssid = "NEATV-AMALIA";     // <<--- ¡ASEGÚRATE DE QUE ESTE WIFI ES CORRECTO!
const char *password = "33am89za"; // <<--- ¡ASEGÚRATE DE QUE ESTA CONTRASEÑA ES CORRECTA!

// --- Objetos y Clientes ---
WebServer server(80); // Servidor en el puerto 80
Servo servoPuerta;
Servo servoRopa;
// CORRECCIÓN RFID: Se asume la configuración estándar de ESP32. SDA/CS en GPIO 5.
MFRC522 rfid(21, 22);    // CS/SDA (GPIO 5), RST (GPIO 22) 

// --- Pines ---
const int LDR_PIN = 34; // Pin para el sensor LDR
const int LED_PATIO = 5; // LED de control automático (Patio) -> ¡ACTUALIZADO A GPIO 5!
const int SENSOR_LLUVIA_PIN = 35; // Pin para el sensor de lluvia
const int RELE_COCINA = 27; // Relevador para la 'luz' principal

const int SERVO_PUERTA_PIN = 13; // Servo para la puerta (Acceso)
const int SERVO_ROPA_PIN = 25;  // Servo para la ropa (Automático Lluvia) 

// --- UID autorizado (¡MODIFICAR!) ---
// UID de tu tarjeta detectada: 33 78 7F F5
const byte UID_AUTORIZADA[] = {0x33, 0x78, 0x7F, 0xF5}; 

// --- Variables de Estado y Auxiliares ---
bool puertaAbierta = false;
unsigned long tiempoApertura = 0;
const unsigned long TIEMPO_CIERRE = 5000; // 5 segundos para cerrar la puerta

// Estados de control para la API y la UI
bool luzCocinaEstado = false;
bool estaLloviendo = false; 
int lecturaLluviaGlobal = 0; // Almacena el valor RAW para debug
bool patioOscuro = false;

// Umbrales (valores analógicos leídos por el ESP32, 0 a 4095)
// Umbral de Lluvia: Valor alto significa lluvia (debido a tu cableado/sensor).
const int UMBRAL_LLUVIA = 1000; 

// Umbral de Oscuridad: Se ha invertido la lógica en el código
// para que el valor ALTO signifique oscuridad (debido a tu cableado LDR).
const int UMBRAL_OSCURIDAD = 500; 

// --- Lógica de Hardware ---

/**
 * Función de prueba para verificar el funcionamiento del servoRopa en el arranque.
 */
void servoTest() {
  Serial.println("⚙️ INICIANDO PRUEBA DEL SERVO DE ROPA (PIN 25)...");
  
  // Mover a 45 grados
  servoRopa.write(45);
  Serial.println("TEST: Escribiendo angulo 45 en servoRopa. Espera 1s.");
  delay(1000); 

  // Mover a 90 grados (Posición de guardado)
  servoRopa.write(90);
  Serial.println("TEST: Escribiendo angulo 90 en servoRopa (GUARDAR). Espera 1s.");
  delay(1000); 

  // Regresar a 0 grados (Posición de extendido)
  servoRopa.write(0);
  Serial.println("✅ PRUEBA DEL SERVO COMPLETADA. Posición final: 0 grados (EXTENDER).");
}


/**
 * Compara el UID leído de la tarjeta con el UID autorizado.
 */
bool compararUID(byte *uid, byte uidSize, const byte *auth, byte authSize) {
  if (uidSize != authSize) return false;
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] != auth[i]) return false;
  }
  return true;
}

/**
 * Abre o cierra el servo de la puerta.
 */
void abrirPuertaServo(int angulo) {
  servoPuerta.write(angulo);
  if (angulo > 0) {
    puertaAbierta = true;
    tiempoApertura = millis();
    Serial.println("✅ Puerta abierta.");
  } else {
    puertaAbierta = false;
    Serial.println("🔒 Puerta cerrada.");
  }
}

/**
 * Cierra automáticamente la puerta después de un tiempo.
 */
void verificarCierrePuerta() {
  if (puertaAbierta && millis() - tiempoApertura > TIEMPO_CIERRE) { 
    abrirPuertaServo(0); // Cierra
  }
}

/**
 * Controla el LED del patio basado en el sensor LDR.
 */
void controlarLDR() {
  int valorLDR = analogRead(LDR_PIN);
  
  // LÓGICA FINAL LDR (Confirmada por el usuario): Mayor al umbral significa OSCURO.
  patioOscuro = (valorLDR > UMBRAL_OSCURIDAD); 
  
  // Activación por LOW (ON)
  digitalWrite(LED_PATIO, patioOscuro ? LOW : HIGH); 
  
  // DEBUG LDR
  Serial.print("DEBUG LDR -> RAW ADC: ");
  Serial.print(valorLDR);
  Serial.print(" | UMBRAL: ");
  Serial.print(UMBRAL_OSCURIDAD);
  Serial.print(" | ¿Oscuro?: ");
  Serial.print(patioOscuro ? "SI" : "NO");
  Serial.print(" | LED PATIO (Pin 5): ");
  Serial.println(patioOscuro ? "ON (LOW)" : "OFF (HIGH)");
}

/**
 * Controla el servo de la ropa basado en el sensor de lluvia.
 */
void controlarLluvia() {
  lecturaLluviaGlobal = analogRead(SENSOR_LLUVIA_PIN);
  
  // CORRECCIÓN FINAL LLUVIA: Usamos >. 
  // La lluvia se detecta cuando el valor es MAYOR que el umbral (debido a la inversión reportada por el usuario).
  estaLloviendo = (lecturaLluviaGlobal > UMBRAL_LLUVIA); // <<< ¡LÓGICA INVERTIDA APLICADA!
  
  int anguloServo;

  if (estaLloviendo) {
    anguloServo = 90;
    Serial.println("--- ESTADO LLUVIA: LLOVIENDO (Guardar Ropa) ---");
  } else {
    anguloServo = 0;
    Serial.println("--- ESTADO LLUVIA: SECO (Extender Ropa) ---");
  }

  servoRopa.write(anguloServo);
  // --- DEBUGGING CRÍTICO ---
  Serial.print("DEBUG Lluvia -> RAW ADC: ");
  Serial.print(lecturaLluviaGlobal);
  Serial.print(" | UMBRAL: ");
  Serial.print(UMBRAL_LLUVIA);
  Serial.print(" | ¿Lloviendo?: ");
  Serial.print(estaLloviendo ? "SI" : "NO");
  Serial.print(" | Angulo Escrito: ");
  Serial.println(anguloServo);
  // -------------------------
}

/**
 * Lee el RFID y abre la puerta si la tarjeta está autorizada.
 */
void controlarRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  // Imprimir el UID LEÍDO para debugging y calibración
  Serial.print("UID LEÍDO (HEX): ");
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX);
  }
  Serial.println(); // Salto de línea después del UID
  // FIN DEBUG

  if (compararUID(rfid.uid.uidByte, rfid.uid.size, UID_AUTORIZADA, sizeof(UID_AUTORIZADA))) {
    abrirPuertaServo(90); // Abre la puerta con RFID
    Serial.println("✅ Tarjeta autorizada. Acceso concedido.");
  } else {
    Serial.println("⛔ Tarjeta no autorizada. Acceso denegado.");
    // Si el UID es diferente, aquí lo verás en el Monitor Serial
  }

  rfid.PICC_HaltA();
}

// --- Manejadores del Servidor Web ---

/**
 * Envía la página HTML completa al cliente.
 */
void handleRoot() {
  server.send(200, "text/html", HTML_CONTENT);
}

/**
 * API: Obtener el estado actual (GET /status)
 */
void handleStatus() {
  // Asegurarse de tener los valores automáticos más recientes
  controlarLDR();
  controlarLluvia(); 
  
  // LDR retorna el valor analógico (0-4095)
  int valorLDR = analogRead(LDR_PIN);
  StaticJsonDocument<256> doc;
  
  // Estado de los sensores
  doc["ldr"] = valorLDR; 
  doc["lluvia"] = estaLloviendo ? "Húmedo" : "Seco";
  doc["lluvia_raw"] = lecturaLluviaGlobal; // ENVIAR VALOR RAW PARA DEBUGGING

  // Estado de control (salidas)
  doc["luz_estado"] = luzCocinaEstado ? "ON" : "OFF";
  doc["puerta_estado"] = puertaAbierta ? "OPEN" : "CLOSED";
  
  String output;
  serializeJson(doc, output);
  
  server.send(200, "application/json", output);
}

/**
 * API: Controlar los dispositivos (POST /control)
 */
void handleControl() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Método no permitido. Usa POST.");
    return;
  }
  
  // Leer el JSON del cuerpo de la petición
  StaticJsonDocument<128> doc;
  // Usamos server.arg("plain") para obtener el cuerpo POST RAW
  DeserializationError error = deserializeJson(doc, server.arg("plain"));
  if (error) {
    server.send(400, "text/plain", "JSON inválido. Error: " + String(error.c_str()));
    return;
  }

  String dispositivo = doc["dispositivo"].as<String>();
  String estado = doc["estado"].as<String>();
  if (dispositivo.isEmpty() || estado.isEmpty()) {
    server.send(400, "text/plain", "Faltan campos 'dispositivo' o 'estado'.");
    return;
  }

  if (dispositivo.equals("luz")) {
    if (estado.equals("ON")) {
      luzCocinaEstado = true;
      digitalWrite(RELE_COCINA, HIGH);
      server.send(200, "text/plain", "Luz Encendida");
    } else if (estado.equals("OFF")) {
      luzCocinaEstado = false;
      digitalWrite(RELE_COCINA, LOW);
      server.send(200, "text/plain", "Luz Apagada");
    } else {
      server.send(400, "text/plain", "Estado de luz inválido.");
    }
  } else if (dispositivo.equals("puerta")) {
    if (estado.equals("OPEN")) {
      abrirPuertaServo(90);
      server.send(200, "text/plain", "Puerta Abierta");
    } else if (estado.equals("CLOSE")) {
      abrirPuertaServo(0); 
      server.send(200, "text/plain", "Puerta Cerrada");
    } else {
      server.send(400, "text/plain", "Estado de puerta inválido.");
    }
  } else {
    server.send(400, "text/plain", "Dispositivo desconocido.");
  }
}

// --- Setup y Loop ---

void setup() {
  Serial.begin(115200);
  // 1. Inicialización de Pines, Servos y RFID
  Serial.println("🔌 Inicializando pines...");
  pinMode(LED_PATIO, OUTPUT);
  pinMode(RELE_COCINA, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(SENSOR_LLUVIA_PIN, INPUT);
  
  // Inicializar salidas apagadas (LOW/HIGH para relé de disparo bajo)
  digitalWrite(LED_PATIO, HIGH); // OFF inicial para relé de disparo LOW
  digitalWrite(RELE_COCINA, LOW); // OFF inicial

  Serial.printf("⚙️ Adjuntando servo de Puerta en pin %d y servo de Ropa en pin %d.\n", SERVO_PUERTA_PIN, SERVO_ROPA_PIN);
  servoPuerta.attach(SERVO_PUERTA_PIN);
  servoRopa.attach(SERVO_ROPA_PIN); 
  servoPuerta.write(0);   
  servoRopa.write(0); // Asegura que empiece en 0 grados
  
  // Ejecuta la prueba del servo al inicio
  servoTest();

  Serial.println("💳 Inicializando RFID...");
  // Se usa SPI.begin() con los pines por defecto (SCK 18, MISO 19, MOSI 23) y se llama a PCD_Init() después.
  // IMPORTANTE: Si los pines SPI de tu ESP32 son diferentes a los por defecto, ajústalos aquí:
  // SPI.begin(SCK, MISO, MOSI, SS/CS);
  SPI.begin(); // Usa los pines SPI por defecto del ESP32 (18, 19, 23)
  rfid.PCD_Init();

  // 2. Conexión Wi-Fi
  Serial.printf("Conectando a Wi-Fi: %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado.");
  
  IPAddress IP = WiFi.localIP();
  Serial.print("Dirección IP de la ESP32: ");
  Serial.println(IP);

  // 3. Configuración del Servidor Web
  server.on("/", handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/control", HTTP_POST, handleControl);

  server.begin();
  Serial.println("🌐 Servidor HTTP iniciado.");
  Serial.println("🏠 Accede a esta IP desde tu navegador para controlar el sistema.");
}

void loop() {
  // Atender peticiones HTTP
  server.handleClient();
  
  // Lógica de automatización continua
  controlarRFID(); // AHORA CON DEBUG DE UID
  verificarCierrePuerta();
  
  // Las funciones de control LDR y Lluvia se llaman en loop para reacción continua
  controlarLDR(); 
  controlarLluvia();
  
  delay(50); // Pequeña pausa
}