ARMGNU = arm-none-eabi
NAME = myfunc
#COPS = -Wall -O2 -mthumb -nostdlib -nostartfiles -ffreestanding
COPS = -mthumb 
all: vectors.o myfunc.o myfunc.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

myfunc.o : myfunc.c 
	$(ARMGNU)-gcc $(COPS) -c myfunc.c -o myfunc.o

myfunc.s : myfunc.c
	$(ARMGNU)-gcc -S $(COPS) -c myfunc.c -o myfunc.s

myfunc.bin : vectors.o myfunc.o memmap
	$(ARMGNU)-ld -T memmap vectors.o myfunc.o -o myfunc.elf
	$(ARMGNU)-objdump -d myfunc.elf > myfunc.list
	$(ARMGNU)-objcopy myfunc.elf myfunc.bin -O binary

clean :
	rm *.o
	rm *.elf
	rm *.bin
