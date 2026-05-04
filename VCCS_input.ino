#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

// --- CALIBRATION SETTINGS ---
const float R_TOP = 10000.0;    
const float R_BOTTOM = 2000.0;  
const float DIVIDER_RATIO = R_BOTTOM / (R_TOP + R_BOTTOM); // 0.1666
const float R_SENSE = 10.0;     // Kept in case you still need it for reference

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Starts the shared I2C bus

  // Initialize DAC (Address 0x61)
  if (!dac.begin(0x61)) { 
    Serial.println("MCP4725 NOT FOUND at 0x61!");
    while (1);
  }

  Serial.println("\n--- 256-STEP VCCS TEST (DAC ONLY) ---");
  Serial.println("Step, Target_V");
}

void loop() {
  // 4096 / 16 = 256 steps
  for (int dacValue = 0; dacValue <= 4096; dacValue += 16) {
    uint32_t safeValue = (dacValue > 4095) ? 4095 : dacValue;
    
    // 1. Set the Voltage
    dac.setVoltage(safeValue, false);

    // 2. Calculate Theoretical Target
    float vDacRaw = (safeValue / 4095.0) * 3.3;
    float vTarget = vDacRaw * DIVIDER_RATIO;

    // 3. Output Data 
    Serial.print(dacValue / 16); Serial.print(", ");
    Serial.println(vTarget, 3); 

    // Wait 1 second before the next step 
    delay(1000); 
  }

  Serial.println("\n--- Sweep Finished. Restarting in 5s ---");
  delay(5000);
}