#!/bin/bash
# ============================================================================
# Name        : make_el3.sh
# Author      : CAP-TEE
# Version     :
# Copyright   : CAP-TEE 2021
# Description : compiles a program, produces an image and runs it on the model at EL3
# =============================================================================

### Check if a directory does not exist ###
if [ ! -d "output" ] 
then
    echo "Creating output directory" 
    mkdir output
fi

# compile program and make elf
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -c -o "output/testloop.o" "testloop.s"
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -Xlinker --entry=main -o "output/testloop.elf"  ./output/testloop.o

# create image
~/projects/baremetalsources/llvm-morello-releases/make-bm-image.sh -i output/testloop.elf -o output/testloop_image_el3

# run model
~/projects/morello_workspace/model/FVP_Morello/models/Linux64_GCC-6.4/FVP_Morello --data Morello_Top.css.scp.armcortexm7ct=/home/osboxes/projects/baremetalsources/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/scp_romfw.bin@0x0 --data Morello_Top.css.mcp.armcortexm7ct=/home/osboxes/projects/baremetalsources/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/mcp_romfw.bin@0x0 -C Morello_Top.soc.scp_qspi_loader.fname=/home/osboxes/projects/baremetalsources/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/scp_fw.bin -C css.scp.armcortexm7ct.INITVTOR=0x0 -C css.mcp.armcortexm7ct.INITVTOR=0x0 --data=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/EL3_s_loop/output/testloop_image_el3@0x14000000 -C css.cluster0.cpu0.semihosting-heap_base=0xB0000000 -C css.cluster0.cpu0.semihosting-heap_limit=0xBE000000 -C css.cluster0.cpu0.semihosting-stack_limit=0xBE000000 -C css.cluster0.cpu0.semihosting-stack_base=0xBFFC0000 --cadi-server
