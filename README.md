# morello-baremetal-examples
This repository contains example code for the Morello Platform. More information regarding these examples can be found in the CAP-TEE Morello Getting Started Guide. https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md

## Prerequisites

It is assumed that you have already installed the Morello FVP (Morello hardware Emulator). See https://github.com/cap-tee/cheri-docs/blob/main/morello/MorelloPlatform/InstallingMorelloFVP.md 

It is assumed that you have already installed the Development Studio (GUI and command line debugger). You will need this to run and debug the example code. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/InstallingArmDevStudio/InstallingArmDevStudio.md

## Directory Structure

### bare-metal-example-scpmcp-binaries:

This directory contains the SCP/MCP binary files in order to run the Morello FVP in bare metal mode. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/BuildOptions/BuildOptions.md. It is also possible to create these files yourself, but takes some time to generate. see https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/YoctoBitbake/YoctoBitbake.md 

### developmentStudio

This directory contains a set of projects that can be loaded into the development Studio. Documentation can be found in the in the CAP-TEE Morello Getting Started Guide https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md.

1. HelloWorld - Outputs "Hello World" to the console at EL3.
2. MMUEL3 - Changes the MMU set up at EL3 for Morello.
3. EL3ToEL1 - Changes Exception level to either EL1S or EL1N.
4. EL3MMUToEL1MMU - Changes Exception level to either EL1S or EL1N and sets up the MMUs, and secure & non-secure memory regions.
5. EL3MMUUART - Changes the MMU set up at EL3 for Morello, sets up the UART and writes a message.
6. EL3MMUToEL1MMUUART - Changes Exception level to either EL1S or EL1N and sets up the MMUs and uart, and writes a message. It also sets up secure & non-secure memory regions.
7. EL3MMUTimerInterrupt - sets up the mmu at EL3, installs the vector tables for exception handling, sets up the interrupt controller, and performs a timer interrupt.
8. EL3MMUEL1MMUUARTtimerInterrupt - sets up the mmu at EL3, installs the vector tables for exception handling, sets up the interrupt controller, and performs a secure timer interrupt. Changes Exception level to either EL1S or EL1N and sets up the MMUs and uart, and performs another timer interrupt (secure timer in EL1S and non secure timer in EL1N). It also sets up secure & non-secure memory regions.
9. EL1ToEL3SMC - example showing how to use the SMC instruction to call into EL3 from either EL1N (normal world) or EL1S (secure world).

### scripts

This contains the run script for the FVP model for bare metal code.