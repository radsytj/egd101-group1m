# Introduction

This project demonstrates use of an L298N motor driver board using an Arduino Uno and a 5V power supply.

# Project Progress

## Initial Setup
- Created a basic Arduino sketch (L298N01.ino) for controlling a single 5V DC motor with the L298N driver, including forward, stop, and backward operations.
- Added a README.md file with wiring instructions.

## Adding Second Motor
- Extended the sketch to control a second 3V DC motor using the L298N's second channel.
- Implemented a voltage divider circuit to reduce the 5V supply to ~3V for the second motor.
- Updated wiring documentation in README.md.

## Integrating Switches and Logic
- Added a power switch (normally open, pin 3) and limit switch (normally open, pin 2) for program control.
- Implemented state machines for:
  - Motor 2: Runs forward until limit switch triggers, holds position for 5 seconds, then reverses until limit is triggered again.
  - Motor 1: Upon limit trigger, runs forward for 2 seconds, pauses for 1 second, then reverses for 2 seconds.
- Added emergency shutdown via power switch off.
- Used millis() for timing and INPUT_PULLUP for switches.

## Fixes and Adjustments
- Adjusted motor 2 reverse logic to stop when the limit switch is triggered again (position-based) instead of time-based, ensuring consistent run times.
- Retained time calculation for reference/debugging.

## Current State
- The sketch is functional with the described behavior.
- Motors: 5V high-speed low-torque (OUT1/OUT2) and 3V high-torque (OUT3/OUT4).
- Configurable delays and speeds are defined as constants.
- Wiring includes voltage divider for 3V motor and switch connections.