# morello-baremetal-examples
This repository contains example code for the Morello Platform. More information regarding these examples can be found in the CAP-TEE Morello Getting Started Guide. https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md. 

Projects that can be compiled in both *Morello* and *Morello-purecap* have a `purecap` label, otherwise they can only be compiled under *Morello* at present.

Note that these examples have been tested with the following combinations:
* Development Studio version `armds-morello-edition-2020-1m0.tgz`, FVP version `FVP_Morello_0.11_9` and LLVM version `bare-metal-release-1.0`.
* Development Studio version `armds-morello-edition-2020-1m0.tgz`, FVP version `FVP_Morello_0.11_19` and LLVM version `bare-metal-release-1.2`.

## Prerequisites

It is assumed that you have already installed the Morello FVP (Morello hardware Emulator). See https://github.com/cap-tee/cheri-docs/blob/main/morello/MorelloPlatform/InstallingMorelloFVP.md 

It is assumed that you have already installed the prebuilt Morello supported LLVM binaries for bare metal. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/LlvmBinaries/LlvmBinaries.md

It is assumed that you have already installed the Development Studio (GUI and command line debugger). You will need this to run and debug the example code. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/InstallingArmDevStudio/InstallingArmDevStudio.md

## Directory Structure

### bare-metal-example-scpmcp-binaries:

This directory contains the SCP/MCP binary files in order to run the Morello FVP in bare metal mode. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/BuildOptions/BuildOptions.md. It is also possible to create these files yourself, but takes some time to generate. see https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/YoctoBitbake/YoctoBitbake.md 

* SCPMCPBuiltWithAPreset0x14000000 - for bare metal firmware starting at EL3
* SCPMCPBuiltWithNoChanges - for bare metal firmware at EL2 (Boot flow examples)
* loopImage - small loop file to run fvp with Development Studio

### developmentStudio

This directory contains a set of projects that can be loaded into the development Studio. Documentation can be found in the in the CAP-TEE Morello Getting Started Guide https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md.

1. HelloWorld - Outputs "Hello World" to the console at EL3. `purecap`
2. EL3-purecap - performs some low level capability tests in assembly to test and explore basic CHERI functionality. `purecap`
3. ChericExample - Exploring the cheri.h API functions and the cheriintrin.h built-in functions. Setting up capability pointers, and retreiving and printing their properties. `purecap`
4. MMUEL3 - Changes the MMU set up at EL3 for Morello. `purecap`
5. EL3ToEL1 - Changes Exception level to either EL1S or EL1N. `purecap`
6. EL3MMUToEL1MMU - Changes Exception level to either EL1S or EL1N and sets up the MMUs, and secure & non-secure memory regions. `purecap`
7. EL3MMUUART - Changes the MMU set up at EL3 for Morello, sets up the UART and writes a message. `purecap`
8. EL3MMUToEL1MMUUART - Changes Exception level to either EL1S or EL1N and sets up the MMUs and uart, and writes a message. It also sets up secure & non-secure memory regions.
9. EL3MMUTimerInterrupt - sets up the mmu at EL3, installs the vector tables for exception handling, sets up the interrupt controller, and performs a timer interrupt.
10. EL3MMUEL1MMUUARTtimerInterrupt - sets up the mmu at EL3, installs the vector tables for exception handling, sets up the interrupt controller, and performs a secure timer interrupt. Changes Exception level to either EL1S or EL1N and sets up the MMUs and uart, and performs another timer interrupt (secure timer in EL1S and non secure timer in EL1N). It also sets up secure & non-secure memory regions.
11. EL1ToEL3SMC - example showing how to use the SMC instruction to call into EL3 from either EL1N (normal world) or EL1S (secure world).
12. EL1NToEL3ToEL1SSMC - example showing how to pass messages between EL1N (normal world) and EL1S (secure world) using SMC.
13. EL2- example showing how to use the EL2N Hypervisor mode to perform a two stage memory translation for EL1N, and restrict EL1N from reading and writing to the EL1N MMU memory registers. 
14. EL3UARTredirect - example showing how to redirect an embedded printf function to the uart. `purecap`

**Boot Flow Examples - EL2N Trusted Firmware Tests (EL2 stand-alone bare metal program):**

1. trustedFW_EL2 - Trusted Firmware Tests. Basic tests to interface to the trusted firmware - this example follows the boot flow to EL2, and then loads an EL2 program from **Development Studio** to interact with the trusted firmware (at EL3).

### commandLine:

This directory contains a set of projects that can be run from the command line. Documentation can be found in the in the CAP-TEE Morello Getting Started Guide https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md.

**Boot Flow Examples - EL3:**

1. EL3_s_loop - Loading an assembly program at EL3.
2. EL3_c_loop - loading programs at EL3.

**Boot Flow Examples - EL2N:**

1. EL2N_s_loop - loading an assembly program at EL2N.
2. EL2N_c_loop - loading programs at EL2N.

**Boot Flow Examples - EL2N Trusted Firmware Tests:**

2. trustedFW_EL2 - Trusted Firmware tests - a script to build, load, and run the EL2 trusted firmware test program from the command line.

### scripts

This contains the run script for the FVP model for bare metal code starting at EL3.