#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS_PIN 5

MCP_CAN CAN(CAN_CS_PIN);
unsigned long lastRequest = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) { }

  Serial.println("Initializing MCP2515...");
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 initialized OK");
  } else {
    Serial.println("MCP2515 init FAILED - check wiring");
  }
  CAN.setMode(MCP_NORMAL);
}

void requestPID(byte pid) {
  byte data[8] = {0x02, 0x01, pid, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN.sendMsgBuf(0x7DF, 0, 8, data);
}

void loop() {
  if (millis() - lastRequest > 500) {
    requestPID(0x0C);
    lastRequest = millis();
  }

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int rxId;
    byte len;
    byte buf[8];
    CAN.readMsgBuf(&rxId, &len, buf);

    if (rxId == 0x7E8) {
      if (buf[2] == 0x0C) {
        int rpm = ((buf[3] * 256) + buf[4]) / 4;
        Serial.print("RPM: ");
        Serial.println(rpm);
      }
    }
  }
}