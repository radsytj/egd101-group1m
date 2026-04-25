# L298N Motor Driver Demo

This project demonstrates controlling a single 5V DC motor using an L298N motor driver board with an Arduino.

## Components Required
- Arduino board (e.g., Uno, Nano)
- L298N motor driver module
- 5V DC motor
- Jumper wires
- Power supply (5V for motor and logic)

## Wiring Instructions

- **Arduino to L298N Connections:**
  - Arduino Pin 8 → L298N IN1
  - Arduino Pin 9 → L298N IN2
  - Arduino Pin 10 → L298N ENA (PWM for speed control)

- **Motor Connections:**
  - Motor positive (+) → L298N OUT1
  - Motor negative (-) → L298N OUT2

- **Power Supply:**
  - For a 5V motor, connect the L298N's 5V input to an external 5V power supply (or Arduino 5V if current is low). Ensure the ground (GND) of the power supply is connected to Arduino GND and L298N GND.
  - Do not power the motor directly from Arduino pins to avoid overloading.

- **Additional Notes:**
  - The L298N requires a separate power source for the motor if it's drawing significant current.
  - Double-check polarities to avoid damaging components.

## Operation
The sketch cycles the motor through:
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