#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#define CS_PIN 5   // GPIO5, chip select pin for the MCP2515 CAN controller

MCP_CAN CAN(CS_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial) { }  // wait for serial monitor

  Serial.println("Initializing MCP2515...");

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 initialized OK");
  } else {
    Serial.println("MCP2515 init FAILED - check wiring");
  }

  CAN.setMode(MCP_NORMAL);
}

void loop() {
  
}