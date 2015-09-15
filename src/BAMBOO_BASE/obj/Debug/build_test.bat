arm-none-eabi-ld -Ttext=0xd102ba00 -o test.elf test.o
arm-none-eabi-objdump -D test.o > test.lst
arm-none-eabi-objcopy -O binary test.elf test.bin
bin2header test.bin
