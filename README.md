🏠 Proyecto: Casa Automática con ESP32
📋 Descripción
Este proyecto simula una casa inteligente automatizada controlada con un ESP32, integrando sensores y actuadores para mejorar la comodidad y eficiencia.
La casa incluye apertura automática de puerta mediante RFID, control de luz con LDR, protección ante lluvia con sensor de humedad, y manejo de luces internas mediante Bluetooth.

⚙️ Funcionalidades
Función	Componente	Descripción
🚪 Puerta automática	RFID RC522 + Servo	Se abre automáticamente al detectar la tarjeta autorizada.
💡 Luz del patio	LDR + LED	Se enciende al detectar poca luz y se apaga cuando hay luz suficiente.
🌧️ Sistema de ropa automática	Sensor de lluvia + Servo	Guarda la ropa en caso de lluvia y la saca cuando se despeja.
🔌 Luz del comedor/cocina	Relé + Bluetooth	Control manual desde el celular mediante conexión Bluetooth.

🧠 Componentes utilizados
Componente	Modelo / Tipo	Función
ESP32	NodeMCU ESP32 DevKit	Microcontrolador principal
RFID	RC522	Detección de tarjetas o llaveros
Sensor de luz	LDR	Control de iluminación exterior
Sensor de lluvia	Analógico	Detección de humedad/agua
Servo motor 1	SG90	Puerta principal
Servo motor 2	SG90	Sistema de ropa
Módulo Relé 1 canal	5V	Luz de cocina/comedor
LED + Resistencia 220Ω	-	Luz del patio
Resistencias varias	10kΩ (LDR), 220Ω (LED)	Circuito base

🔌 Conexiones
Función	Componente	Pin ESP32	Notas
RFID SDA	RC522	21	Comunicación SPI
RFID SCK	RC522	18	Comunicación SPI
RFID MOSI	RC522	23	Comunicación SPI
RFID MISO	RC522	19	Comunicación SPI
RFID RST	RC522	22	Reset del módulo
Servo puerta	SG90	13	Apertura controlada por RFID
Servo ropa	SG90	14	Movimiento automático según lluvia
LDR	Sensor de luz	34 (analógico)	Entrada de luz ambiental
LED patio	LED	2	Luz automática del exterior
Sensor lluvia	Analógico	35	Entrada analógica
Relé cocina	Módulo 1 canal	27	Controlado por Bluetooth
VCC / GND	Todos	3.3V / 5V y GND	Según componente

🔐 UID Autorizada
Solo se abrirá la puerta con la siguiente tarjeta RFID:
const byte UID_AUTORIZADA[] = {0xC3, 0x07, 0x0A, 0xFD};

📲 Control Bluetooth
Conectá tu celular al dispositivo CasitaESP32 y usá cualquier app serial Bluetooth (por ejemplo, Serial Bluetooth Terminal).
Comandos disponibles:
•	Enviar 1 → Enciende luz del comedor/cocina.
•	Enviar 0 → Apaga luz del comedor/cocina.

🧩 Librerías necesarias
Asegurate de tener instaladas las siguientes librerías desde el Administrador de Librerías del IDE de Arduino:
•	ESP32Servo
•	BluetoothSerial
•	SPI
•	MFRC522

🧰 Instrucciones de uso
1.	Cargar el código Casa_Automatica_ESP32.ino en el ESP32.
2.	Conectar todos los componentes según la tabla de conexiones.
3.	Alimentar el ESP32 (por USB o fuente externa de 5V).
4.	Probar el sistema:
o	Pasar tu tarjeta RFID autorizada para abrir la puerta.
o	Cubrir el LDR para encender el LED del patio.
o	Simular lluvia (conectar a GND) para activar el servo de la ropa.
o	Usar Bluetooth para encender/apagar la luz del comedor.

🧪 Simulación en Proteus
El proyecto puede simularse en Proteus 8.15 o superior, usando el modelo del ESP32 (modificado o genérico) y módulos equivalentes a los sensores.
Asegurate de conectar los pines tal como en la tabla anterior.

👩‍💻 Autores
Luciana Zapana y Brandon Cisneros
Tecnicatura Universitaria en Automatización y Robótica
Proyecto “Casa Automática” — 2025

