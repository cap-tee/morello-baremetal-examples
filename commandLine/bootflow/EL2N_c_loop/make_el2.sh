#!/bin/bash
# ============================================================================
# Name        : make_el3.sh
# Author      : CAP-TEE
# Version     :
# Copyright   : CAP-TEE 2021
# Description : compiles a program, produces an image and runs it on the model at EL2
# =============================================================================

### Check if a directory does not exist ###
if [ ! -d "output" ] 
then
    echo "Creating output directory" 
    mkdir output
fi
# compile program and make elf
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -O0 -g -MMD -MP -c -o "output/testloopmain.o" "testloopmain.c"
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -c -o "output/testloop.o" "testloop.s"
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -Xlinker --entry=main -o "output/testloop.elf"  ./output/testloopmain.o output/testloop.o

# create image
~/projects/baremetalsources/llvm-morello-releases/make-bm-image_el2.sh -i output/testloop.elf -o output/testloop_image_el2

# run model
~/projects/morello_workspace/model/FVP_Morello/models/Linux64_GCC-6.4/FVP_Morello --data Morello_Top.css.scp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/scp_romfw.bin@0x0 --data Morello_Top.css.mcp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/mcp_romfw.bin@0x0 -C Morello_Top.soc.scp_qspi_loader.fname=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/scp_fw.bin -C Morello_Top.soc.mcp_qspi_loader.fname=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/mcp_fw.bin -C css.scp.armcortexm7ct.INITVTOR=0x0 -C css.mcp.armcortexm7ct.INITVTOR=0x0 --data=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/EL2N_c_loop/output/testloop_image_el2@0x14200000 -C css.pl011_uart_ap.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/EL2N_c_loop/output/uart0-bootflow.log -C css.scp.pl011_uart_scp.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/EL2N_c_loop/output/scp-bootflow.log -C css.mcp.pl011_uart0_mcp.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/EL2N_c_loop/output/mcp-bootflow.log -C css.pl011_uart_ap.unbuffered_output=1 -C displayController=0 -C board.virtio_rng.enabled=1 -C board.virtio_rng.seed=0 -C num_clusters=2 -C num_cores=2 -C board.hostbridge.userNetworking=true -C board.smsc_91c111.enabled=true -C board.hostbridge.userNetPorts="5555=5555" --cadi-server

