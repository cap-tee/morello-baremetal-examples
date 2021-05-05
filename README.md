# morello-baremetal-examples
This repository contains example code for the Morello Platform. More information regarding these examples can be found in the CAP-TEE Morello Getting Started Guide. https://github.com/cap-tee/cheri-docs/blob/main/morello-getting-started.md

## Prerequisites

It is assumed that you have already installed the Morello FVP (Morello hardware Emulator). See https://github.com/cap-tee/cheri-docs/blob/main/morello/MorelloPlatform/InstallingMorelloFVP.md 

It is assumed that you have already installed the Development Studio (GUI and command line debugger). You will need this to run and debug the example code. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/InstallingArmDevStudio/InstallingArmDevStudio.md

## Directory Structure

### bare-metal-example-scpmcp-binaries:

This directory contains the SCP/MCP binary files in order to run the Morello FVP in bare metal mode. See https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/BuildOptions/BuildOptions.md. It is also possible to create these files yourself, but takes some time to generate. see https://github.com/cap-tee/cheri-docs/blob/main/morello/BareMetalOnMorello/YoctoBitbake/YoctoBitbake.md 

### developmentStudio

This directory contains a set of projects that can be loaded into the development Studio. Documentation can be found in the in the CAP-TEE Morello Getting Started Guide.

1. HelloWorld - Outputs "Hello World" to the console at EL3.
2. MMUEL3 - Changes the MMU set up at EL3 for Morello.
3. EL3ToEL1 - Changes Exception level to either EL1S or EL1N.


### scripts

This contains the run script for the FVP model for bare metal code.