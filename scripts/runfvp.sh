
./../../morello_workspace/model/FVP_Morello/models/Linux64_GCC-6.4/FVP_Morello --data Morello_Top.css.scp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/scp_romfw.bin@0x0 --data Morello_Top.css.mcp.armcortexm7ct=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/mcp_romfw.bin@0x0 -C Morello_Top.soc.scp_qspi_loader.fname=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/SCPMCPBuiltWithAPreset0x14000000/scp_fw.bin -C css.scp.armcortexm7ct.INITVTOR=0x0 -C css.mcp.armcortexm7ct.INITVTOR=0x0 --data=/home/osboxes/projects/morello-baremetal-examples/bare-metal-example-scpmcp-binaries/loopImage/loop_image_el3@0x14000000 -C css.cluster0.cpu0.semihosting-heap_base=0xB0000000 -C css.cluster0.cpu0.semihosting-heap_limit=0xBE000000 -C css.cluster0.cpu0.semihosting-stack_limit=0xBE000000 -C css.cluster0.cpu0.semihosting-stack_base=0xBFFC0000 --run --cadi-server
