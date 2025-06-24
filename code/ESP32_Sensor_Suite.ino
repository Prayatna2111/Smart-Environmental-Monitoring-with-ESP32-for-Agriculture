#include <DHT.h>
#include <ModbusMaster.h>

// ---------------- PH SENSOR ----------------
const int PH_PIN = 35;
float V7 = 2.5;
float S = 0.1;

// --------------- DHT22 SENSOR ---------------
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ----------- SOIL MOISTURE & LDR -----------
#define SOIL_PIN 34
#define LDR_PIN  32

// ---------------- RS485 NPK ----------------
#define MAX485_DE_RE 5
#define RXD2 16
#define TXD2 17
ModbusMaster node;

// Convert two 16-bit registers to IEEE754 float
float convertRegistersToFloat(uint16_t high, uint16_t low) {
  uint32_t combined = ((uint32_t)high << 16) | low;
  float value;
  memcpy(&value, &combined, sizeof(value));
  return value;
}

// ---------------- SETUP ----------------
void preTransmission() {
  digitalWrite(MAX485_DE_RE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE, 0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogReadResolution(12); // ESP32 12-bit ADC
  dht.begin();

  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, 0);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("✅ All sensors initialized.");
}

// ---------------- LOOP ----------------
void loop() {
  Serial.println("\n📡 Reading sensor data...");

  // ---------- PH SENSOR ----------
  int phRaw = analogRead(PH_PIN);
  float voltage = phRaw * (3.3 / 4095.0);
  float pH = 7.0 + ((V7 - voltage) / S);
  String pHtype = (pH < 3) ? "Strongly Acidic" :
                  (pH < 6) ? "Weakly Acidic" :
                  (pH < 7) ? "Slightly Acidic" :
                  (pH == 7) ? "Neutral" :
                  (pH > 12) ? "Strongly Alkaline" :
                  (pH > 9) ? "Weakly Alkaline" : "Slightly Alkaline";
  Serial.print("🔬 pH: "); Serial.print(pH, 2); Serial.print(" (unitless) | ");
  Serial.print("Voltage: "); Serial.print(voltage, 3); Serial.print(" V | ");
  Serial.print("Type: "); Serial.println(pHtype);

  // ---------- DHT22 ----------
  float tempC = dht.readTemperature();
  float hum = dht.readHumidity();
  float tempF = tempC * 9.0 / 5.0 + 32.0;
  if (isnan(tempC) || isnan(hum)) {
    Serial.println("❌ Failed to read from DHT22");
  } else {
    Serial.print("🌡 Temperature: ");
    Serial.print(tempC); Serial.print(" °C / ");
    Serial.print(tempF); Serial.println(" °F");
    Serial.print("💧 Humidity: ");
    Serial.print(hum); Serial.println(" % RH");
  }

  // ---------- SOIL MOISTURE ----------
  int soilRaw = analogRead(SOIL_PIN);
  soilRaw = constrain(soilRaw, 1000, 4095);
  int soilPercent = map(soilRaw, 2800, 1300, 0, 100);
  Serial.print("🌱 Soil Moisture: ");
  Serial.print(soilPercent); Serial.println(" %");

  // ---------- LDR ----------
  int ldrRaw = analogRead(LDR_PIN);
  int lightPercent = map(ldrRaw, 0, 4095, 100, 0);
  Serial.print("💡 Light Level: ");
  Serial.print(lightPercent); Serial.println(" % brightness");

  // ---------- NPK SENSOR ----------
  Serial.println("🔄 Reading NPK values...");
  uint8_t result = node.readHoldingRegisters(0x0001, 6);
  if (result == node.ku8MBSuccess) {
    float N = convertRegistersToFloat(node.getResponseBuffer(0), node.getResponseBuffer(1));
    float P = convertRegistersToFloat(node.getResponseBuffer(2), node.getResponseBuffer(3));
    float K = convertRegistersToFloat(node.getResponseBuffer(4), node.getResponseBuffer(5));
    Serial.print("🌾 Nitrogen (N): "); Serial.print(N, 2); Serial.println(" mg/kg");
    Serial.print("🌾 Phosphorus (P): "); Serial.print(P, 2); Serial.println(" mg/kg");
    Serial.print("🌾 Potassium (K): "); Serial.print(K, 2); Serial.println(" mg/kg");
  } else {
    Serial.print("❌ Modbus Error: 0x"); Serial.println(result, HEX);
    if (result == 0xE0) {
      Serial.println("🔎 Tip: Check sensor ID, baudrate, or register map.");
    }
  }

  Serial.println("📥 Waiting 10 seconds...\n");
  delay(10000);
}
