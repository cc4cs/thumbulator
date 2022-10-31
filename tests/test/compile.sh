#!/bin/bash
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m0 -o test_program.o test_program.c
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m0 -o startup.o startup.c
arm-none-eabi-ld -Tstm32.ld -o test_program.elf startup.o test_program.o
arm-none-eabi-objcopy -O binary test_program.elf test_program.bin

