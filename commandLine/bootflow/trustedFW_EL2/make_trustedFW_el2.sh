#!/bin/bash
# ============================================================================
# Name        : make_trustedFW_el2.sh
# Author      : CAP-TEE
# Version     :
# Copyright   : CAP-TEE 2021
# Description : compiles the trusted firmware test program, produces an image 
#               and runs it on the model at EL2
# =============================================================================

### Check if a directory does not exist ###
if [ ! -d "output" ] 
then
    echo "Creating output directory" 
    mkdir output
fi


~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -c -o "output/el2nmmusetup_allDRAM.o" "../../../developmentStudio/trustedFW_EL2/src/el2nmmusetup_allDRAM.s"

~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -O0 -g -MMD -MP -c -o "output/printf.o" "../../../developmentStudio/trustedFW_EL2/src/printf.c"
 
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -c -o "output/setupel2.o" "../../../developmentStudio/trustedFW_EL2/src/setupel2.S"
 
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -O0 -g -MMD -MP -c -o "output/trustedFW_EL2.o" "../../../developmentStudio/trustedFW_EL2/src/trustedFW_EL2.c"
 
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -c -o "output/trustedFW_EL2asmfuncs.o" "../../../developmentStudio/trustedFW_EL2/src/trustedFW_EL2asmfuncs.s"
 
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -O0 -g -MMD -MP -c -o "output/trustedFW_EL2funcs.o" "../../../developmentStudio/trustedFW_EL2/src/trustedFW_EL2funcs.c"
 
~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -O0 -g -MMD -MP -c -o "output/uart_redirect.o" "../../../developmentStudio/trustedFW_EL2/src/uart_redirect.c"

~/projects/baremetalsources/llvm-morello-releases/bin/clang --target=aarch64-none-elf -march=morello -Xlinker -T/home/osboxes/projects/morello-baremetal-examples/developmentStudio/trustedFW_EL2/src/linker-script-el2.ld -v -Xlinker --entry=_startel2 -o "output/trustedFW_EL2.elf"  ./output/el2nmmusetup_allDRAM.o ./output/printf.o ./output/setupel2.o ./output/trustedFW_EL2.o ./output/trustedFW_EL2asmfuncs.o ./output/trustedFW_EL2funcs.o ./output/uart_redirect.o   

 "/home/osboxes/projects/baremetalsources/llvm-morello-releases/bin/ld.lld" -o output/trustedFW_EL2.elf -entry _start --image-base 0x80000000 -L /home/osboxes/projects/baremetalsources/llvm-morello-releases/aarch64-none-elf+morello+a64c/lib --gc-sections /home/osboxes/projects/baremetalsources/llvm-morello-releases/aarch64-none-elf+morello+a64c/lib/rdimon-crt0.o /home/osboxes/projects/baremetalsources/llvm-morello-releases/aarch64-none-elf+morello+a64c/lib/crti.o /home/osboxes/projects/baremetalsources/llvm-morello-releases/aarch64-none-elf+morello+a64c/lib/cpu-init/rdimon-aem-el3.o -T/home/osboxes/projects/morello-baremetal-examples/developmentStudio/trustedFW_EL2/src/linker-script-el2.ld --entry=_startel2 ./output/el2nmmusetup_allDRAM.o ./output/printf.o ./output/setupel2.o ./output/trustedFW_EL2.o ./output/trustedFW_EL2asmfuncs.o ./output/trustedFW_EL2funcs.o ./output/uart_redirect.o --start-group -lc -lrdimon --end-group -lclang_rt.builtins-aarch64 /home/osboxes/projects/baremetalsources/llvm-morello-releases/aarch64-none-elf+morello+a64c/lib/crtn.o

# create image
~/projects/baremetalsources/llvm-morello-releases/make-bm-image_el2.sh -i output/trustedFW_EL2.elf -o output/trustedFW_EL2_image

# run model
~/projects/morello_workspace/model/FVP_Morello/models/Linux64_GCC-6.4/FVP_Morello --data Morello_Top.css.scp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/scp_romfw.bin@0x0 --data Morello_Top.css.mcp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/mcp_romfw.bin@0x0 -C Morello_Top.soc.scp_qspi_loader.fname=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/scp_fw.bin -C Morello_Top.soc.mcp_qspi_loader.fname=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithNoChanges/mcp_fw.bin -C css.scp.armcortexm7ct.INITVTOR=0x0 -C css.mcp.armcortexm7ct.INITVTOR=0x0 --data=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/trustedFW_EL2/output/trustedFW_EL2_image@0x14200000 -C css.pl011_uart_ap.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/trustedFW_EL2/output/uart0-bootflow.log -C css.scp.pl011_uart_scp.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/trustedFW_EL2/output/scp-bootflow.log -C css.mcp.pl011_uart0_mcp.out_file=/home/osboxes/projects/morello-baremetal-examples/commandLine/bootflow/trustedFW_EL2/output/mcp-bootflow.log -C css.pl011_uart_ap.unbuffered_output=1 -C displayController=0 -C board.virtio_rng.enabled=1 -C board.virtio_rng.seed=0 -C num_clusters=2 -C num_cores=2 -C board.hostbridge.userNetworking=true -C board.smsc_91c111.enabled=true -C board.hostbridge.userNetPorts="5555=5555" --run --cadi-server

