# compilation of dts to dtb:
# dtc -I dts -O dtb -o output.dtb input.dts

#decompilation:
dtc -I dtb -O dts -o output.dts input.dtb
