#include <SPI.h>
#include <mcp_can.h>

// Chip select pin for MCP2515
#define CS_CHIP 10
MCP_CAN CAN0(CS_CHIP);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize MCP2515 at 500 kbps with 8 MHz crystal
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok!");
  } else {
    Serial.println("CAN init fail, check wiring!");
    while (1); // Traps code in infinite loop because of error
  }

  // Enter loopback mode (self-test mode)
  CAN0.setMode(MCP_LOOPBACK);
  Serial.println("Entered Loopback mode.");
}

void loop() {
  unsigned char data[8] = {10, 20, 30, 40, 50, 60, 70, 80};
  unsigned long txId = 0x100;

  // Send a CAN frame
  byte sendStatus = CAN0.sendMsgBuf(txId, 0, 8, data);
  if (sendStatus == CAN_OK) {
    Serial.println("Message sent in loopback mode!");
  } else {
    Serial.println("Error sending message!");
  }

  // Wait for a message to appear in receive buffer
  if (CAN0.checkReceive() == CAN_MSGAVAIL) {
    unsigned long rxId;
    unsigned char len;
    unsigned char rxBuf[8];

    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    Serial.print("Received message ID: 0x");
    Serial.println(rxId, HEX);
    Serial.print("Data: ");
    for (int i = 0; i < len; i++) {
      Serial.print(rxBuf[i]);
      Serial.print(" ");
    }
    Serial.println("\n------------------");
  }

  delay(1000);
}
