# L298N Motor Driver Demo

This project demonstrates controlling two DC motors (5V and 3V) using an L298N motor driver board with an Arduino.

## Components Required
- Arduino board (e.g., Uno, Nano)
- L298N motor driver module
- 5V DC motor
- 3V DC motor
- Jumper wires
- Power supply (5V for motor and logic)

## Wiring Instructions

- **Arduino to L298N Connections:**
  - Arduino Pin 8 → L298N IN1 (Motor 1)
  - Arduino Pin 9 → L298N IN2 (Motor 1)
  - Arduino Pin 10 → L298N ENA (Motor 1 PWM)
  - Arduino Pin 11 → L298N IN3 (Motor 2)
  - Arduino Pin 12 → L298N IN4 (Motor 2)
  - Arduino Pin 13 → L298N ENB (Motor 2 PWM)

- **Motor Connections:**
  - Motor 1 (5V) positive (+) → L298N OUT1
  - Motor 1 (5V) negative (-) → L298N OUT2
  - Motor 2 (3V) positive (+) → L298N OUT3
  - Motor 2 (3V) negative (-) → L298N OUT4

- **Power Supply:**
  - Connect L298N Vs (motor power) to a 5V supply.
  - For Motor 2 (3V), use a voltage divider to drop the 5V to ~3V: Connect a 2kΩ resistor from 5V supply to L298N Vs, and a 3kΩ resistor from L298N Vs to GND. This provides ~3V at Vs for Motor 2 (note: Vs is shared, so Motor 1 will also see ~3V—use separate L298N modules for different voltages if needed).
  - Connect Arduino GND to L298N GND and power supply GND.

- **Additional Notes:**
  - The voltage divider reduces the supply voltage for Motor 2. Adjust resistor values for exact 3V (e.g., R1=2kΩ, R2=3kΩ gives 5V * (3k/(2k+3k)) ≈ 3V).
  - PWM in the code is adjusted for Motor 2 to simulate lower voltage operation.

## Operation
The sketch cycles the motors through:
1. Forward rotation at half speed (2 seconds)
2. Stop (1 second)
3. Backward rotation at half speed (2 seconds)
4. Stop (1 second)

Repeat indefinitely.

## Motor Control Logic
- **Forward:** IN1 = HIGH, IN2 = LOW
- **Backward:** IN1 = LOW, IN2 = HIGH
- **Stop:** IN1 = LOW, IN2 = LOW
- **Speed:** Controlled via PWM on ENA pin (0-255)

## Safety Notes
- Ensure the motor voltage matches your power supply.
- Do not exceed the L298N's current rating (typically 2A per channel).
- Use appropriate heat sinks if driving high-current motors.