// ==========================================
// SMART HOME AUTOMATION - JAVASCRIPT
// ==========================================

// Configuración del ESP32
const ESP32_IP = '192.168.1.27'; // Cambiar por la IP de tu ESP32
const ESP32_URL = `http://${ESP32_IP}`;

// Variables de estado
let deviceStates = {
    luz: 'OFF',
    puerta: 'OFF',
    ldr: 0,
    lluvia: 'Seco'
};

let isConnected = false;
let lastUpdateTime = null;

// Elementos del DOM
const elements = {
    // Sensores
    ldrValue: document.getElementById('ldrValue'),
    ldrBar: document.getElementById('ldrBar'),
    rainStatus: document.getElementById('rainStatus'),
    rainIcon: document.getElementById('rainIcon'),
    
    // Controles
    lightStatus: document.getElementById('lightStatus'),
    lightStatusDot: document.getElementById('lightStatusDot'),
    doorStatus: document.getElementById('doorStatus'),
    doorStatusDot: document.getElementById('doorStatusDot'),
    
    // Estado de conexión
    connectionStatus: document.getElementById('connectionStatus'),
    connectionDot: document.getElementById('connectionDot'),
    lastUpdate: document.getElementById('lastUpdate'),
    
    // Botones
    lightOnBtn: document.getElementById('lightOnBtn'),
    lightOffBtn: document.getElementById('lightOffBtn'),
    doorOnBtn: document.getElementById('doorOnBtn'),
    doorOffBtn: document.getElementById('doorOffBtn'),
    
    // Fecha y hora
    currentDateTime: document.getElementById('currentDateTime')
};

// ==========================================
// FUNCIONES DE UTILIDAD
// ==========================================

// Actualizar fecha y hora
function updateDateTime() {
    const now = new Date();
    const options = {
        weekday: 'long',
        year: 'numeric',
        month: 'long',
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit'
    };
    
    if (elements.currentDateTime) {
        elements.currentDateTime.textContent = now.toLocaleDateString('es-ES', options);
    }
}

// Formatear tiempo transcurrido
function formatTimeAgo(date) {
    if (!date) return 'Nunca';
    
    const now = new Date();
    const diff = now - date;
    const seconds = Math.floor(diff / 1000);
    
    if (seconds < 60) return 'Hace unos segundos';
    if (seconds < 3600) return `Hace ${Math.floor(seconds / 60)} minutos`;
    if (seconds < 86400) return `Hace ${Math.floor(seconds / 3600)} horas`;
    
    return `Hace ${Math.floor(seconds / 86400)} días`;
}

// ==========================================
// FUNCIONES DE CONEXIÓN
// ==========================================

// Enviar comando al ESP32
async function sendCommandToESP32(endpoint) {
    try {
        console.log(`Enviando comando a: ${ESP32_URL}${endpoint}`);
        
        const response = await fetch(`${ESP32_URL}${endpoint}`, {
            method: 'GET',
            mode: 'no-cors', // Cambiar para evitar problemas de CORS
            cache: 'no-cache'
        });
        
        // No verificar response.ok debido a mode: 'no-cors'
        console.log('Comando enviado exitosamente');
        
        // Actualizar estado inmediatamente después de enviar comando
        setTimeout(fetchSensorData, 500);
        
        return true;
        
    } catch (error) {
        console.error('Error al enviar comando:', error);
        showNotification('Error al conectar con ESP32', 'error');
        return false;
    }
}

// Obtener datos del ESP32
async function fetchSensorData() {
    try {
        console.log('Obteniendo datos del ESP32...');
        
        const response = await fetch(`${ESP32_URL}/estado`, {
            method: 'GET',
            mode: 'cors',
            cache: 'no-cache',
            headers: {
                'Accept': 'application/json'
            }
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const data = await response.json();
        console.log('Datos recibidos:', data);
        
        // Actualizar estados (mapear nombres del ESP32 a nuestros nombres)
        deviceStates.ldr = data.ldr || 0;
        deviceStates.luz = data.led === 1 ? 'ON' : 'OFF';
        deviceStates.puerta = data.rele === 1 ? 'ON' : 'OFF';
        deviceStates.lluvia = data.lluvia || 'Seco';
        isConnected = true;
        lastUpdateTime = new Date();
        
        // Actualizar interfaz
        updateUI();
        updateConnectionStatus(true);
        
    } catch (error) {
        console.error('Error al obtener datos:', error);
        isConnected = false;
        updateConnectionStatus(false);
        
        // En modo desconectado, simular datos
        simulateDisconnectedData();
    }
}

// Simular datos cuando está desconectado
function simulateDisconnectedData() {
    console.log('Simulando datos desconectados...');
    
    // Simular valores aleatorios
    deviceStates.ldr = Math.floor(Math.random() * 4096);
    deviceStates.lluvia = Math.random() > 0.7 ? 'Lloviendo' : 'Seco';
    
    // Mantener últimos estados conocidos de dispositivos
    // o generar estados aleatorios
    if (Math.random() > 0.8) {
        deviceStates.luz = deviceStates.luz === 'ON' ? 'OFF' : 'ON';
    }
    if (Math.random() > 0.9) {
        deviceStates.puerta = deviceStates.puerta === 'ON' ? 'OFF' : 'ON';
    }
    
    updateUI();
}

// ==========================================
// ACTUALIZACIÓN DE LA INTERFAZ
// ==========================================

// Actualizar estado de conexión
function updateConnectionStatus(connected) {
    if (elements.connectionStatus) {
        if (connected) {
            elements.connectionStatus.textContent = 'Conectado';
            elements.connectionStatus.classList.remove('disconnected');
            elements.connectionDot.classList.remove('disconnected');
        } else {
            elements.connectionStatus.textContent = 'Desconectado';
            elements.connectionStatus.classList.add('disconnected');
            elements.connectionDot.classList.add('disconnected');
        }
    }
    
    if (elements.lastUpdate) {
        elements.lastUpdate.textContent = `Última actualización: ${formatTimeAgo(lastUpdateTime)}`;
    }
}

// Actualizar interfaz de usuario
function updateUI() {
    // Actualizar sensor LDR
    if (elements.ldrValue) {
        elements.ldrValue.textContent = deviceStates.ldr;
    }
    
    if (elements.ldrBar) {
        const percentage = (deviceStates.ldr / 4096) * 100;
        elements.ldrBar.style.width = `${percentage}%`;
    }
    
    // Actualizar estado de lluvia
    if (elements.rainStatus) {
        elements.rainStatus.textContent = deviceStates.lluvia;
    }
    
    if (elements.rainIcon) {
        if (deviceStates.lluvia === 'Lloviendo') {
            elements.rainIcon.className = 'fas fa-cloud-rain weather-icon';
            elements.rainIcon.style.color = '#00d4ff';
        } else {
            elements.rainIcon.className = 'fas fa-sun weather-icon';
            elements.rainIcon.style.color = '#00ff88';
        }
    }
    
    // Actualizar estado de luz
    if (elements.lightStatus) {
        elements.lightStatus.textContent = deviceStates.luz === 'ON' ? 'Encendida' : 'Apagada';
    }
    
    if (elements.lightStatusDot) {
        if (deviceStates.luz === 'ON') {
            elements.lightStatusDot.classList.add('active');
        } else {
            elements.lightStatusDot.classList.remove('active');
        }
    }
    
    // Actualizar estado de puerta
    if (elements.doorStatus) {
        elements.doorStatus.textContent = deviceStates.puerta === 'ON' ? 'Abierta' : 'Cerrada';
    }
    
    if (elements.doorStatusDot) {
        if (deviceStates.puerta === 'ON') {
            elements.doorStatusDot.classList.add('active');
        } else {
            elements.doorStatusDot.classList.remove('active');
        }
    }
}

// ==========================================
// FUNCIONES DE CONTROL
// ==========================================

// Control de luz
async function controlLight(action) {
    const endpoint = `/rele/${action}`;
    const success = await sendCommandToESP32(endpoint);
    
    if (success) {
        deviceStates.luz = action === 'on' ? 'ON' : 'OFF';
        updateUI();
        showNotification(`Luz ${action === 'on' ? 'encendida' : 'apagada'}`, 'success');
    }
}

// Control de puerta
async function controlDoor(action) {
    const endpoint = `/puerta/${action}`;
    const success = await sendCommandToESP32(endpoint);
    
    if (success) {
        deviceStates.puerta = action === 'on' ? 'ON' : 'OFF';
        updateUI();
        showNotification(`Puerta ${action === 'on' ? 'abierta' : 'cerrada'}`, 'success');
    }
}

// ==========================================
// NOTIFICACIONES
// ==========================================

function showNotification(message, type = 'info') {
    // Crear elemento de notificación
    const notification = document.createElement('div');
    notification.className = `notification notification-${type}`;
    notification.textContent = message;
    
    // Estilos para la notificación
    notification.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 1rem 1.5rem;
        border-radius: 10px;
        color: white;
        font-weight: 600;
        z-index: 10000;
        transform: translateX(100%);
        transition: transform 0.3s ease;
        max-width: 300px;
        word-wrap: break-word;
        ${type === 'success' ? 'background: linear-gradient(45deg, #00ff88, #00d4ff);' : ''}
        ${type === 'error' ? 'background: linear-gradient(45deg, #ff3366, #ff6b6b);' : ''}
        ${type === 'info' ? 'background: linear-gradient(45deg, #00d4ff, #6a00ff);' : ''}
    `;
    
    // Agregar al body
    document.body.appendChild(notification);
    
    // Animar entrada
    setTimeout(() => {
        notification.style.transform = 'translateX(0)';
    }, 100);
    
    // Remover después de 3 segundos
    setTimeout(() => {
        notification.style.transform = 'translateX(100%)';
        setTimeout(() => {
            if (notification.parentNode) {
                notification.parentNode.removeChild(notification);
            }
        }, 300);
    }, 3000);
}

// ==========================================
// FUNCIONES DE ANIMACIÓN
// ==========================================

// Animación de scroll
function initScrollAnimations() {
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };
    
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add('animated');
            }
        });
    }, observerOptions);
    
    // Observar elementos
    document.querySelectorAll('.animate-on-scroll').forEach(el => {
        observer.observe(el);
    });
}

// ==========================================
// EVENT LISTENERS
// ==========================================

// Agregar event listeners cuando el DOM esté cargado
document.addEventListener('DOMContentLoaded', function() {
    console.log('Smart Home Automation iniciado');
    
    // Botones de control de luz
    if (elements.lightOnBtn) {
        elements.lightOnBtn.addEventListener('click', () => controlLight('on'));
    }
    
    if (elements.lightOffBtn) {
        elements.lightOffBtn.addEventListener('click', () => controlLight('off'));
    }
    
    // Botones de control de puerta
    if (elements.doorOnBtn) {
        elements.doorOnBtn.addEventListener('click', () => controlDoor('on'));
    }
    
    if (elements.doorOffBtn) {
        elements.doorOffBtn.addEventListener('click', () => controlDoor('off'));
    }
    
    // Inicializar animaciones
    initScrollAnimations();
    
    // Mostrar notificación de inicio
    showNotification('Sistema Smart Home iniciado', 'info');
});

// ==========================================
// INICIALIZACIÓN
// ==========================================

// Iniciar actualizaciones periódicas
function startPeriodicUpdates() {
    // Actualizar fecha y hora cada segundo
    setInterval(updateDateTime, 1000);
    updateDateTime();
    
    // Actualizar datos de sensores cada 3 segundos
    setInterval(fetchSensorData, 3000);
    
    // Actualizar estado de conexión cada 5 segundos
    setInterval(() => {
        updateConnectionStatus(isConnected);
    }, 5000);
}

// Iniciar cuando la página esté cargada
window.addEventListener('load', function() {
    startPeriodicUpdates();
    
    // Cargar datos iniciales
    setTimeout(fetchSensorData, 1000);
});

// ==========================================
// FUNCIONES ADICIONALES
// ==========================================

// Navegación responsive
function initNavigation() {
    const navToggle = document.querySelector('.nav-toggle');
    const navMenu = document.querySelector('.nav-menu');
    
    if (navToggle && navMenu) {
        navToggle.addEventListener('click', () => {
            navMenu.classList.toggle('active');
        });
        
        // Cerrar menú al hacer clic en un enlace
        document.querySelectorAll('.nav-link').forEach(link => {
            link.addEventListener('click', () => {
                navMenu.classList.remove('active');
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

// Inicializar navegación
initNavigation();

// ==========================================
// MANEJO DE ERRORES GLOBAL
// ==========================================

window.addEventListener('error', function(e) {
    console.error('Error global:', e.error);
    showNotification('Error en la aplicación', 'error');
});

window.addEventListener('unhandledrejection', function(e) {
    console.error('Promesa rechazada no manejada:', e.reason);
    showNotification('Error de conexión', 'error');
});