#include <inttypes.h>

static const uint8_t G_N_PIN = 10;
static const uint8_t DS_PIN = 11;
static const uint8_t STCP_PIN = 12;
static const uint8_t SHCP_PIN = 13;

uint16_t readData()
{
  digitalWrite(G_N_PIN, LOW);
  delay(1);
  uint16_t data = 0x0000 | PINF | (PINB << 8);
  digitalWrite(G_N_PIN, HIGH);
  return data;
}

void outputAddress(uint32_t addr)
{
  digitalWrite(STCP_PIN, LOW);
  shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, (addr >> 16));
  shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, (addr >> 8));
  shiftOut(DS_PIN, SHCP_PIN, MSBFIRST, addr);
  digitalWrite(STCP_PIN, HIGH);
}

void dump(uint32_t addr, uint32_t size)
{
  uint16_t data;
  for (uint32_t addrIdx = addr; addrIdx < addr + size / 2; addrIdx++) {
    outputAddress(addrIdx);
    data = readData();
    Serial.print((char)(data));
    Serial.print((char)(data >> 8));
  }
  Serial.println();
}

void setup()
{
  pinMode(G_N_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);

  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  DDRF = 0x00;
  DDRB = 0x00;

  Serial.begin(115200);
}

void loop()
{
  char commandBuffer[64];
  if (Serial.available()) {
    char c = '\0';
    uint8_t i = 0;
    while (i < 64) {
      while (!Serial.available())
        ;
      c = Serial.read();
      if (c == '\r' || c == '\n') {
        commandBuffer[i] = '\0';
        break;
      }
      commandBuffer[i] = c;
      i++;
    }

    uint32_t addr, size;
    if (sscanf(commandBuffer, "DUMP %lu %lu", &addr, &size) == 2) {
      dump(addr, size);
    } else {
      Serial.println("ERROR");
    }
  }
}
