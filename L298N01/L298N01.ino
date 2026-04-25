// L298N Motor Driver Demo for Single 5V DC Motor
// This sketch demonstrates basic motor control: forward, stop, backward
// Wiring: See README.md for detailed connections.

// Define pins for L298N
#define IN1 8   // Input 1 for motor direction
#define IN2 9   // Input 2 for motor direction
#define ENA 10  // Enable pin for motor speed (PWM)

void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Start with motor stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);  // Speed 0
}

void loop() {
  // Motor forward at half speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 128);  // Half speed (0-255)
  delay(2000);  // Run for 2 seconds

  // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  delay(1000);  // Stop for 1 second

  // Motor backward at half speed
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 128);
  delay(2000);  // Run for 2 seconds

  // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  delay(1000);  // Stop for 1 second
}