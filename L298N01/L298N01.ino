// Define pins for first motor (small motor)
#define IN1 8   // Input 1 for motor 1 direction
#define IN2 9   // Input 2 for motor 1 direction
#define ENA 10  // Enable pin for motor speed (PWM)

// Define pins for second motor (large motor)
#define IN3 11  // Input 3 for motor 2 direction
#define IN4 12  // Input 4 for motor 2 direction
#define ENB 13  // Enable pin for second motor speed (PWM)

// Define pins for switches (normally open, using INPUT_PULLUP)
#define POWER_SWITCH 3  // Toggle switch for power
#define LIMIT_SWITCH 2  // Microswitch for limit

// Configurable delays (in milliseconds)
#define HOLD_DELAY 4000  // Delay before motor 2 reverses
#define HOLD_DELAY_SHORT 1000  // Shorter delay for pause between 2 and 5 second runs of motor 2
#define MOTOR1_FORWARD_TIME 1000  // Motor 1 forward run time
#define MOTOR1_PAUSE_TIME 1000  // Motor 1 pause time (added to motor2ForwardDuration to act as a buffer)
#define MOTOR1_REVERSE_TIME 1000  // Motor 1 reverse run time

// Motor speeds
#define MOTOR1_SPEED 255  // Full speed for 5V motor
#define MOTOR2_SPEED 200  // Reduced speed for 3V motor

// Debug flag
#define DEBUG_SERIAL true  // Enables serial debug output

// State variables
int motor2State = 0; // 0: off/idle, 1: motor2 to limit, 2: holding, 3: reversing, 4: done
int motor1State = 0; // 0: idle, 1: forward, 2: pause, 3: reverse, 4: done
bool lastPowerState = false; // Track previous power state to detect cycling
bool waitingForReset = false; // Flag for waiting after cycle complete
unsigned long powerOnTime;
unsigned long limitHitTime;
unsigned long holdStartTime;
unsigned long reverseStartTime;
unsigned long motor1StartTime;
unsigned long motor2ForwardDuration;



void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  if (DEBUG_SERIAL) {
    Serial.println("\n=== L298N Motor Control Starting ===");
  }

  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start with motors stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);  // Speed 0
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);  // Speed 0

  // Initialize switches
  pinMode(POWER_SWITCH, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  
  if (DEBUG_SERIAL) {
    Serial.println("Setup complete. Waiting for power switch...");
  }
}

void loop() {
  bool powerOn = (digitalRead(POWER_SWITCH) == LOW);  // LOW when on (closed)
  bool limitTriggered = (digitalRead(LIMIT_SWITCH) == LOW);  // LOW when triggered (closed)

  // Check if we're in "waiting for reset" state
  if (waitingForReset) {
    if (powerOn && !lastPowerState) {
      // Power just turned back ON - reset and ready for next cycle
      if (DEBUG_SERIAL) {
        Serial.println("\n[RESET] Power cycled - Ready for next cycle\n");
      }
      waitingForReset = false;
      motor2State = 0;
      motor1State = 0;
    }
    lastPowerState = powerOn;
    return;  // Stay in this state until power is cycled
  }

  if (!powerOn) {
    // Emergency shutdown: stop all motors and reset
    if (motor2State != 0 || motor1State != 0) {
      if (DEBUG_SERIAL) {
        Serial.println("[POWER OFF] Emergency shutdown - stopping all motors");
      }
    }
    stopAllMotors();
    motor2State = 0;
    motor1State = 0;
    lastPowerState = false;
    return;
  }
  
  lastPowerState = true;  // Track that power is currently on

  // Motor 2 state machine
  if (motor2State == 0) {
    // Power just turned on: start motor 2 forward
    if (DEBUG_SERIAL) {
      Serial.println("[STATE 0→1] Power on - Starting Motor 2 forward");
    }
    startMotor2Forward();
    powerOnTime = millis();
    motor2State = 1;
  } else if (motor2State == 1) {
    // Running to limit
    if (limitTriggered) {
      limitHitTime = millis();
      motor2ForwardDuration = limitHitTime - powerOnTime;
      if (DEBUG_SERIAL) {
        Serial.print("[STATE 1→2] Limit triggered - Motor 2 forward duration: ");
        Serial.print(motor2ForwardDuration);
        Serial.println(" ms");
      }
      holdMotor2();
      holdStartTime = millis();
      motor2State = 2;
      // Start motor 1 sequence
      if (DEBUG_SERIAL) {
        Serial.println("[MOTOR1] Starting Motor 1 forward sequence");
      }
      startMotor1Forward();
      motor1StartTime = millis();
      motor1State = 1;
    } 
  } else if (motor2State == 2) {
    // Holding position
    if (millis() - holdStartTime >= HOLD_DELAY) {
      if (DEBUG_SERIAL) {
        Serial.println("[STATE 2→3] Hold delay complete - Starting Motor 2 reverse");
      }
      startMotor2Reverse();
      reverseStartTime = millis();
      motor2State = 3;
    }
  } else if (motor2State == 3) {
    // Reversing for the same duration as forward
    if (millis() - reverseStartTime >= 2000) {
      if (DEBUG_SERIAL) {
        Serial.println("[STATE 3→4] Reverse complete - Motor 2 stopped (cycle done)");
      }
      holdMotor2();
      holdStartTime = millis();
      motor2State = 4;
    }
  } else if (motor2State == 4) {
    //Pause again
    if (millis() - holdStartTime >= HOLD_DELAY_SHORT) {
      if (DEBUG_SERIAL) {
        Serial.println("[STATE 4→5] Hold delay complete - Starting Motor 2 final step");
      }
      powerOnTime = millis();
      startMotor2Forward();
      motor2State = 5;
    }
  } else if (motor2State == 5) {
    // Final forward run for 5 seconds
    if (millis() - powerOnTime >= 5000) {
      if (DEBUG_SERIAL) {
        Serial.println("[STATE 5→6] Final forward complete - Motor 2 stopped (cycle done)");
      }
      stopMotor2();
      motor2State = 6;  // Done
    }
  }
  // State 6: Done, no further action

  // Motor 1 sequence (independent, triggered by limit)
  if (motor1State == 1) {
    // Forward
    if (millis() - motor1StartTime >= MOTOR1_FORWARD_TIME) {
      if (DEBUG_SERIAL) {
        Serial.println("[MOTOR1 1→2] Forward complete - Motor 1 pausing");
      }
      stopMotor1();
      motor1State = 2;
      motor1StartTime = millis();
    }
  } else if (motor1State == 2) {
    // Pause
    if (millis() - motor1StartTime >= MOTOR1_PAUSE_TIME) {
      if (DEBUG_SERIAL) {
        Serial.println("[MOTOR1 2→3] Pause complete - Motor 1 reversing");
      }
      startMotor1Reverse();
      motor1State = 3;
      motor1StartTime = millis();
    }
  } else if (motor1State == 3) {
    // Reverse
    if (millis() - motor1StartTime >= MOTOR1_REVERSE_TIME) {
      stopMotor1();
      motor1State = 4;  // Done
      if (DEBUG_SERIAL) {
        Serial.println("[MOTOR1 3→4] Reverse complete - Motor 1 stopped (sequence done)");
      }
    }
  } 
  // Motor1State 4: Done, no further action

  // Check if both motors are complete - enter waiting for reset state
  if (motor2State == 6 && motor1State == 4 && !waitingForReset) {
    if (DEBUG_SERIAL) {
      Serial.println("\n[COMPLETE] Both motors finished - Waiting for power switch reset");
      Serial.println("Turn power OFF then back ON to run another cycle\n");
    }
    waitingForReset = true;
  }

  // Delay to prevent watchdog timer reset and allow serial buffer to flush
  delay(5);
}

// Motor control functions
void startMotor1Forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR1_SPEED);
}

void startMotor1Reverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, MOTOR1_SPEED);
}

void stopMotor1() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void startMotor2Forward() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, MOTOR2_SPEED);
}

void holdMotor2() {
  // Braking: short the motor windings
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);  // Full enable for braking
}

void startMotor2Reverse() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, MOTOR2_SPEED);
}

void stopMotor2() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void stopAllMotors() {
  stopMotor1();
  stopMotor2();
}