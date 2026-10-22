# Embedded Firmware — HIFU Device

**HIFU Device · Electronics, Firmware and HMI Development**

## Overview

The firmware targets the **STM32F103C8T6** and is supplied as an STM32CubeMX/STM32 HAL project with a Keil µVision (`MDK-ARM`) project configuration. The original application source files are preserved without changes.

## Project Contents

| Path | Purpose |
| --- | --- |
| [`Hifu_mixed/Core/Src/main.c`](Hifu_mixed/Core/Src/main.c) | Main application and device-control logic. |
| [`Hifu_mixed/Core/`](Hifu_mixed/Core/) | Application source and headers, peripheral initialization, and interrupt routines. |
| [`Hifu_mixed/Hifu_mixed.ioc`](Hifu_mixed/Hifu_mixed.ioc) | STM32CubeMX pin and peripheral configuration. |
| [`Hifu_mixed/MDK-ARM/Hifu_mixed.uvprojx`](Hifu_mixed/MDK-ARM/Hifu_mixed.uvprojx) | Keil µVision project file. |
| [`Hifu_mixed/Backup_codes/code_backup.c`](Hifu_mixed/Backup_codes/code_backup.c) | Original archived application source snapshot. |
| [`Hifu_mixed/Drivers/`](Hifu_mixed/Drivers/) | Required HAL/CMSIS components with original third-party notices. |

## Implemented Functions

The firmware includes **UART communication with the touchscreen and other device electronics**, I²C communication, timer/PWM peripherals, GPIO control, and application logic for status indicators, cartridge/handpiece states, session counters, and timing management.

## Source Preservation

Application source and project configuration were copied byte-for-byte from the supplied archive. Generated intermediate build artifacts, firmware output binaries, and machine-specific editor state are not included. Third-party STMicroelectronics and Arm CMSIS components remain subject to their supplied license terms.

## Related Documentation

[Project overview](../README.md) · [Hardware development](../hardware/README.md) · [HMI development](../hmi/README.md)
