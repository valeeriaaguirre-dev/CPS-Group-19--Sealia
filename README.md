# Sealia

An interactive self-balancing robotic seal with integrated machine learning.
## Overview

Sealia is an interactive robotic seal that combines mechanical movement, ball interaction, sound, computer vision, and machine learning. Sealia uses a platform to balance different color balls on its nose, based on the color of the ball, 3 different actions occur. 

The project is based on the open-source [BABOT project by JohanLink](https://github.com/JohanLink/BABOT).

---

## Interaction Behaviours

Sealia currently responds to three classes.

| Class | Input | Behaviour |
| --- | --- | --- |
| Class 1 | 🔴 Red Ball | Happy behaviour |
| Class 2 | 🟢 Green Ball | Launch behaviour |
| Class 3 | 🔵 Blue Ball | Performance behaviour |

## Servo and Output Pins

| Component | Arduino Pin | Movement |
| --- | --- | --- |
| Platform Servo D | Pin 2 | 0° up / 90° down |
| Platform Servo E | Pin 3 | 0° up / 90° down |
| Platform Servo F | Pin 4 | 0° up / 90° down |
| Tail Servo C | Pin 9 | 100° closed / 170° open |
| Left Arm A | Pin 10 | 45° down / 130° up |
| Right Arm B | Pin 11 | 45° down / 130° up |
| Buzzer | Pin 12 | Audio output |

The seal arms begin at approximately 80°. The Arduino gradually moves the arm servos one degree at a time toward their target positions to create smoother movements.

### **Class 1 — Red Ball**

When the red ball is detected:

1. Platform servos D, E and F move to 90°, keeping the ball platform flat in the lowered position.
2. Arm A on Pin 10 moves between 45° and 130°.
3. Arm B on Pin 11 moves between 45° and 130°.
4. Both arms move together, creating a synchronized waving motion.
5. The tail servo on Pin 9 repeatedly moves between 100° and 170°, creating the clapping motion.
6. Sealia plays the Happy Song through the buzzer on Pin 12.

### **Class 2 — Green Ball**

When the green ball is detected:

1. The tail closes to 100° using the servo on Pin 9.
2. Arm A and Arm B slowly move upward toward 130°.
3. While the arms are moving, platform servos D, E and F remain at 90°.
4. The Arduino waits until both seal arms have reached 130°.
5. Platform servos D and E then move from 90° to 0°, creating the ball-launching movement.
6. At the same time, the tail opens from 100° to 170°.
7. Sealia then plays the Sad Song through the buzzer on Pin 12.

### **Class 3 — Blue Ball**

When the blue ball is detected:

1. Platform servos D, E and F move to 0°, keeping the platform flat in its raised position.
2. Arm A moves toward 130° while Arm B moves toward 45°.
3. Once both arms reach their target positions, the targets switch.
4. Arm A then moves toward 45° while Arm B moves toward 130°.
5. This continues repeatedly, creating an alternating waving motion.
6. The tail servo repeatedly moves between 100° and 170°, creating the clapping motion.
7. Audio is played through the buzzer on Pin 12.

---

## Machine Learning

Sealia uses Wekinator as its machine-learning environment. The model is trained using RGB values captured from the external webcam integrated into the ball platform, training examples are recorded for each of the three coloured balls.

```text
R > G and R > B
→ Class 1
→ Red Ball

G > R and G > B
→ Class 2
→ Green Ball

B > R and B > G
→ Class 3
→ Blue Ball
```

Wekinator then outputs the corresponding class to Processing.

---

## Software Pipeline

Sealia uses Wekinator, Processing, and Arduino together as one software pipeline.

```text
External Webcam
      ↓
RGB Input
      ↓
Wekinator
      ↓
Class 1 / 2 / 3
      ↓
Processing
      ↓
Serial Communication
      ↓
Arduino
      ↓
Physical Performance
```

### Wekinator

Wekinator is responsible for classifying the RGB input.

It has been trained using examples from the red, green, and blue balls.

The output is one of three classes.

```text
1 → Red
2 → Green
3 → Blue
```

### Processing

Processing connects Wekinator to the Arduino.

The Processing script receives the class detected by Wekinator.

It then converts that class into a serial message and sends it to the Arduino.

```text
Wekinator
    ↓
Detected Class
    ↓
Processing
    ↓
Serial Message
    ↓
Arduino
```

### Arduino

The Arduino receives the serial message and selects the corresponding performance.

It controls the physical outputs of Sealia, including:

- Arm movement
- Tail movement
- Ball platform position
- Audio playback
- Timing between movements

---

## Mechanical Design

Sealia is based on the mechanical foundation of BABOT for its ball-balancig platform, which has been modified to create a robotic seal character.

The robot includes:

- Moving arms
- Articulated tail
- Ball platform
- Integrated webcam position
- Internal servo and actuator mounts
- Custom seal-inspired outer body

The ball platform forms an important part of both the mechanical and interaction design, it supports the ball during interaction, contains the webcam used for colour detection, and can change position depending on the performance being executed. The mechanical structure also allows Sealia to communicate through physical gestures rather than only through movement of the balancing platform.

There are 3 different gear components powered by servo motors. 2 servo motors which power each arm of the seal. and One servo component which powers the tail. The gears and body of the seal have been 3D printed using a combination of  a Recycled PLA with wood fibers blend and wood-fiber. 

---

## Hardware

Sealia combines computer-side machine learning with Arduino-based hardware control.

Main system components include:

```text
Microcontroller:
Arduino

Machine Learning:
Wekinator

Camera:
External Webcam

Communication Software:
Processing

Mechanism:
Servos Motors and Custom Gear Components

Sound:
Passive Buzzer

Power Supply:
External Battery 5V

---

## System Architecture

The complete Sealia system can be represented as:

```text
                     SEALIA

                   USER INPUT
                       │
                       ▼
                Coloured Ball
                       │
                       ▼
              External Webcam
                       │
                       ▼
                  RGB Values
                       │
                       ▼
                   Wekinator
                       │
                       ▼
               Class Detection
                       │
                       ▼
                   Processing
                       │
                Serial Message
                       │
                       ▼
                    Arduino
                       │
         ┌─────────────┼─────────────┐
         │             │             │
         ▼             ▼             ▼
       Arms           Tail        Platform
                                      │
                                      ▼
                                  Ball Balancing

                       │
                       ▼
                     Audio
```

---

## Project Status

Current functionality:

- External webcam input
- RGB colour detection
- Wekinator training
- Three-class classification
- Colour ball recognition
- Processing communication
- Serial communication with Arduino
- Arm movement
- Tail movement
- Ball platform control for balance or "launch". 
- Audio playback
- Three individual performances

The current prototype is functional and can recognise the three coloured balls and perform the corresponding programmed behaviour. The self balancing element using IR sensors and actuated platform balancing, was not completed for the submission of this project and will be an element added to our future prototype.  

---

## Future Development

Future development could include:

- A larger machine-learning training dataset
- Improved performance under different lighting conditions
- IR Sensor Board for actuated platform balancing 

A larger training dataset containing examples from different lighting environments could make the colour classification more reliable in different lighting conditions. The current three-class system could also be extended so that Sealia responds to additional objects or interactions rather than only colour. The self balancing platform with further custom hardware will be completed and improved for a future version of this project. 

---

## Based on BABOT

Sealia is based on the open-source BABOT project created by JohanLink.

Original project:

[https://github.com/JohanLink/BABOT](https://github.com/JohanLink/BABOT)

BABOT provides the foundation for the ball-balancing robot platform.

Sealia develops this platform further through:

- A custom seal-inspired body
- Modified mechanical components
- Moving arms
- An articulated tail
- A ball launching system
- Webcam integration
- RGB-based interaction
- Wekinator machine learning
- Processing communication
- Custom Arduino behaviours
- Sound
- Character-driven performances

Credit for the original BABOT platform remains with its original creator and contributors.

---

## Documentation

More detailed project documentation can be stored inside the `docs` directory with images and further explanations. 

---

## Authors

Sealia Project

Adam Saint-Jaques, Amber Jiang &  Valeria Aguirre

---

## Licence and Attribution

Sealia is a student project created for the module Cyber Physical Systems at Imperial College. London, UK. 2026.
It uses the open-source project [BABOT](https://github.com/JohanLink/BABOT) for the self balancing platform. 
