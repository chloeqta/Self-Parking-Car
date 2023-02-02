# Self-Parking-Car

## Introduction
This project describes an Arduino self-parking car that uses ultrasonic sensors. This car is capable of driving parallel along a wall until it detects an open parking spot, where it can automatically park itself.

## Hardware components
- Arduino IDE
- Robot car chasis kit
- H-Bridge motor control
- 3 HC-SR04 Ultrasonic distance sensors
- 3 HC-SR04 sensor mounts
- 6 AA battery holder

## Software
The overall control logic consists of three parts: parking spot detection, turn movement, and halting at the end of a spot. Inputs from the two inline ultrasonic sensors are used for integral control and self-adjustment from a wall. Major changes in the frontmost inline sensor indicates a right angle turn into a spot and motor stoppage once the front sensor reads a value less than 7 cm.
