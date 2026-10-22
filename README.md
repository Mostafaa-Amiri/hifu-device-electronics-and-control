# HIFU Device — Electronics, Firmware and HMI Development

**Electronics and Embedded Systems Engineering**  
*Reverse Engineering · Dual-Board PCB Design · STM32 Firmware · Touchscreen HMI*

## Overview

This repository documents the **reverse engineering, redesign, development, and improvement** of the electronic control platform of a **high-intensity focused ultrasound (HIFU) device**. The engineering work includes two custom, interconnected PCBs in a stacked arrangement, analog and digital circuit development, **STM32F103C8T6** firmware, touchscreen human–machine interface (HMI) design, and integration of the assembled device.

The repository serves as a technical portfolio of the implemented electronics and embedded-control system. It contains original firmware source code, PCB design renders, assembly photographs, HMI screenshots, and a video of the developed device in operation. **Editable PCB/schematic design files and native HMI project files are intentionally not published.**

## System Architecture

The electronics use **two stacked, interconnected circuit boards** integrating analog and digital functions. An STM32F103C8T6 microcontroller coordinates peripheral interfaces and exchanges information with the DWIN touchscreen HMI.

The firmware project includes UART, I²C, GPIO, DMA, and timer/PWM configuration, with application logic for interface communication, handpiece/cartridge-related state handling, status indicators, timing, and operating counters. These describe the documented embedded implementation rather than clinical or therapeutic performance.

## Hardware Development and Integration

The figures below show the two custom PCB designs and their physical implementation.

<p align="center">
  <img src="hardware/images/main-board-altium-3d.png" alt="3D PCB render of the HIFU main circuit board" width="49%">
  <img src="hardware/images/second-board-altium-3d.png" alt="3D PCB render of the second HIFU circuit board" width="49%">
</p>

<p align="center"><em>3D views of the two custom-designed circuit boards.</em></p>

<p align="center">
  <img src="hardware/images/stacked-boards-assembled.png" alt="Assembled stacked HIFU circuit boards" width="49%">
  <img src="hardware/images/stacked-boards-side-view.jpg" alt="Side view of the interconnected PCB assembly" width="49%">
</p>

<p align="center"><em>Populated and interconnected boards in the stacked hardware assembly.</em></p>

The developed electronics were integrated with the device enclosure and touchscreen. Further PCB photographs and an additional EMI/EMC-related prototype are documented in [Hardware Development](hardware/README.md).

## Device Demonstration

The following video shows the **developed HIFU device in operation**, as demonstrated by its developer. It provides a visual record of the assembled system and its touchscreen interface; it is not a clinical efficacy or regulatory-compliance demonstration.

<p align="center">
  <a href="https://youtube.com/shorts/ZNVhH4hBH6o?feature=share"><img src="hmi/images/hmi-operational-screen.png" alt="Watch the developed HIFU device demonstration on YouTube" width="320"></a>
</p>

**[Watch the working-device demonstration on YouTube](https://youtube.com/shorts/ZNVhH4hBH6o?feature=share)** · [Results and Media](results-and-media/README.md)

## Engineering Contributions

- **PCB and circuit development:** redesign of two custom, vertically stacked PCBs incorporating analog and digital circuitry, physical assembly, and inter-board connections.
- **Embedded firmware:** STM32F103C8T6 application development in C using STM32CubeMX, STM32 HAL, and Keil µVision.
- **Touchscreen HMI:** interface design and iteration using DWIN DGUS tools, with integration into the device control platform.
- **Reverse engineering and system integration:** analysis of existing electronic interfaces, implementation of revised hardware and software, and integration of the developed subsystems.
- **Prototyping and investigation:** an additional PCB for EMI/EMC-related development work, documented through renders and photographs.

## Repository Sections

| Section | Description |
| --- | --- |
| **[Hardware](hardware/README.md)** | PCB design renders, populated boards, stacked assembly, and EMI/EMC-related prototype photographs. |
| **[Embedded Firmware](firmware/README.md)** | STM32F103C8T6 C application code, STM32CubeMX configuration, Keil project, and associated HAL/CMSIS components. |
| **[Touchscreen HMI](hmi/README.md)** | DWIN DGUS interface design screenshots, hardware integration, and operational touchscreen photographs. |
| **[Results and Media](results-and-media/README.md)** | Working-device demonstration video and integrated equipment photographs. |

---

## Copyright and Citation

Copyright © 2026 Mostafa Amiri. All rights reserved for original contributions.

This repository is publicly accessible for professional portfolio and engineering documentation purposes. No general open-source license is granted for original materials. Third-party libraries and other third-party materials remain subject to their respective licenses and rights.

For reuse permissions, see [Copyright and Permissions](COPYRIGHT.md). For attribution, see [CITATION.cff](CITATION.cff).

---

**Engineering, development, implementation, and documentation:** [Mostafa Amiri](https://github.com/Mostafaa-Amiri).
