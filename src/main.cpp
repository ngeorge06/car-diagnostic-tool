#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS_PIN 5

MCP_CAN CAN(CAN_CS_PIN);

unsigned long captureStart = 0;
const unsigned long CAPTURE_DURATION = 30000; // 30 seconds
bool capturing = false;

void printFrame(unsigned long timestamp, long unsigned int id, byte len, byte* buf) {
  Serial.print(timestamp);
  Serial.print("ms  ID: 0x");
  Serial.print(id, HEX);
  Serial.print("  Data:");
  for (int i = 0; i < len; i++) {
    Serial.print(" ");
    if (buf[i] < 0x10) Serial.print("0");
    Serial.print(buf[i], HEX);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { }

  Serial.println("Initializing MCP2515...");
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 initialized OK");
  } else {
    Serial.println("MCP2515 init FAILED - check wiring");
  }

  CAN.setMode(MCP_NORMAL); // instead of MCP_LISTENONLY
  Serial.println();
  Serial.println("Pure passive capture - nothing will be sent.");
  Serial.println("Starting in 2 seconds...");
  delay(2000);

  Serial.println(">>> LISTENING NOW <<<");
  captureStart = millis();
  capturing = true;
}

void loop() {
  if (capturing) {
    unsigned long elapsed = millis() - captureStart;

    if (elapsed >= CAPTURE_DURATION) {
      capturing = false;
      Serial.println(">>> CAPTURE COMPLETE - nothing was transmitted during this test <<<");
      return;
    }

    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      long unsigned int rxId;
      byte len;
      byte buf[8];
      CAN.readMsgBuf(&rxId, &len, buf);
      printFrame(elapsed, rxId, len, buf);
    }
  }
}