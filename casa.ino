#include <BluetoothSerial.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// --- Objetos ---
BluetoothSerial SerialBT;
Servo servoPuerta;
Servo servoRopa;
MFRC522 rfid(21, 22);  // SDA, RST

// --- Pines ---
const int LDR_PIN = 34;
const int LED_PATIO = 2;
const int SENSOR_LLUVIA = 35;
const int RELE_COCINA = 27;

const int SERVO_PUERTA_PIN = 13;
const int SERVO_ROPA_PIN = 14;

// --- UID autorizado ---
const byte UID_AUTORIZADA[] = {0xC3, 0x07, 0x0A, 0xFD}; // Tu tarjeta válida

// --- Variables auxiliares ---
bool puertaAbierta = false;
unsigned long tiempoApertura = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("CasitaESP32");

  // Inicialización RFID
  SPI.begin(18, 19, 23, 21); // SCK, MISO, MOSI, SDA
  rfid.PCD_Init();

  // Pines
  pinMode(LED_PATIO, OUTPUT);
  pinMode(RELE_COCINA, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(SENSOR_LLUVIA, INPUT);

  // Servos
  servoPuerta.attach(SERVO_PUERTA_PIN);
  servoRopa.attach(SERVO_ROPA_PIN);

  servoPuerta.write(0);  // Puerta cerrada
  servoRopa.write(0);    // Ropa afuera

  Serial.println("🏠 Sistema de casa automática con Bluetooth y RFID listo.");
}

void loop() {
  controlarLDR();
  controlarLluvia();
  controlarRFID();
  controlarBluetooth();
  verificarCierrePuerta();
}

// --- LDR controla LED del patio ---
void controlarLDR() {
  int valorLDR = analogRead(LDR_PIN);
  if (valorLDR < 2000) {
    digitalWrite(LED_PATIO, HIGH);
  } else {
    digitalWrite(LED_PATIO, LOW);
  }
}

// --- Sensor de lluvia controla servo ropa ---
void controlarLluvia() {
  int lluvia = analogRead(SENSOR_LLUVIA);
  if (lluvia < 1500) {
    servoRopa.write(90); // Guarda la ropa
  } else {
    servoRopa.write(0);  // Saca la ropa
  }
}

// --- RFID abre puerta solo con tarjeta autorizada ---
void controlarRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  if (compararUID(rfid.uid.uidByte, rfid.uid.size, UID_AUTORIZADA, sizeof(UID_AUTORIZADA))) {
    Serial.println("✅ Tarjeta autorizada: abriendo puerta...");
    abrirPuerta();
  } else {
    Serial.println("⛔ Tarjeta no autorizada.");
  }

  rfid.PICC_HaltA();
}

// --- Bluetooth controla relé del comedor/cocina ---
void controlarBluetooth() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '1') {
      digitalWrite(RELE_COCINA, HIGH);
      SerialBT.println("💡 Luz cocina encendida");
    } else if (c == '0') {
      digitalWrite(RELE_COCINA, LOW);
      SerialBT.println("💡 Luz cocina apagada");
    }
  }
}

// --- Comparar UID de tarjeta RFID ---
bool compararUID(byte *uid, byte uidSize, const byte *auth, byte authSize) {
  if (uidSize != authSize) return false;
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] != auth[i]) return false;
  }
  return true;
}

// --- Apertura y cierre automático de la puerta ---
void abrirPuerta() {
  servoPuerta.write(90); // Abrir
  puertaAbierta = true;
  tiempoApertura = millis();
}

void verificarCierrePuerta() {
  if (puertaAbierta && millis() - tiempoApertura > 5000) { // 5 segundos
    servoPuerta.write(0); // Cerrar
    puertaAbierta = false;
    Serial.println("🔒 Puerta cerrada automáticamente.");
  }
}
