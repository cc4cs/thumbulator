ARMGNU = arm-none-eabi
NAME = 
COPS = -mthumb 

all: vectors.o $(NAME).o $(NAME).bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

$(NAME).o : $(NAME).c 
	$(ARMGNU)-gcc $(COPS) -c $(NAME).c -o $(NAME).o

$(NAME).s : $(NAME).c
	$(ARMGNU)-gcc -S $(COPS) -c $(NAME).c -o $(NAME).s

$(NAME).bin : vectors.o $(NAME).o memmap
	$(ARMGNU)-ld -T memmap vectors.o $(NAME).o -o $(NAME).elf
	$(ARMGNU)-objdump -d $(NAME).elf > $(NAME).list
	$(ARMGNU)-objcopy $(NAME).elf $(NAME).bin -O binary

clean :
	rm *.o
	rm *.elf
	rm *.bin
