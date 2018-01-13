# Inactive

USB keyboard controller firmware for STM32 microcontollers

**Old description:**

This is a USB keyboard controller firmware for STM32 contollers with USB-FS hardware (USB-OTG isn't supporter at the moment).

Primary development MCU is STM32F103, should be easy to port to STM32F102, STM32L1xx and STM32F3xx.

There are two main parts: boot and core. Boot supports basic keyboard functionality and firmware upgrade via USB (DFU protocol). Cores provide advanced features.

**Features:**
* DFU firmware upgrade
* ACM/CDC serial debug interface
