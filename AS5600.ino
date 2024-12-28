#include <Wire.h>
#include <AS5600.h>

#define AS5600_BURN 0xFF // Burn register address

AS5600 encoder; // Create an AS5600 object

void setup() {
    Wire.begin(); // Initialize I2C
    Serial.begin(9600); // Initialize Serial Communication
    Serial.println("Programming AS5600...");

    // Step 1: Check if the magnet is detected
    if (!encoder.detectMagnet()) {
        Serial.println("No magnet detected! Adjust placement and try again.");
        while (1); // Halt further execution
    }
    Serial.println("Magnet detected. Proceeding with programming...");

    // Step 2: Program the zero position (ZPOS)
    programZeroPosition();

    // Step 3: Set maximum angle (MANG) to 360 degrees
    programMaximumAngle(360);

    // Step 4: Burn the settings into OTP memory
    burnSettings();
    Serial.println("Programming complete. Encoder set as analog potentiometer (0-360°).");
}

void loop() {
    // Do nothing
}

// Function to program the zero position (ZPOS)
void programZeroPosition() {
    uint16_t rawAngle = encoder.rawAngle(); // Get current RAW ANGLE
    encoder.setZPosition(rawAngle); // Program zero position
    Serial.println("Zero position programmed.");
}

// Function to program the maximum angle (MANG)
void programMaximumAngle(uint16_t maxAngle) {
    uint16_t mangValue = (maxAngle * 4096) / 360; // Scale maxAngle to AS5600 units
    encoder.setMaxAngle(mangValue); // Program maximum angle
    Serial.println("Maximum angle set to 360°.");
}

// Function to burn settings into OTP memory
void burnSettings(){
  writeReg(AS5600_BURN, 0x80);
  writeReg(AS5600_BURN, 0x40);
  delay(5);
  writeReg(AS5600_BURN, 0x01);
  writeReg(AS5600_BURN, 0x11);
  writeReg(AS5600_BURN, 0x10);
  delay(5);
}


// Function to write a value to an AS5600 register
bool writeReg(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(encoder.getAddress()); // Get AS5600 I2C address
    Wire.write(reg); // Register address
    Wire.write(value); // Value to write
    return (Wire.endTransmission() == 0); // Return true if successful
}
