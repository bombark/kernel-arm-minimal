all:
	arm-none-eabi-as -mcpu=arm926ej-s src/startup.s -o startup.s.o
	arm-none-eabi-gcc -c -mcpu=arm926ej-s -o main.o  src/main.c
	arm-none-eabi-ld -T etc/main.ld main.o startup.s.o -L /usr/lib/gcc/arm-none-eabi/6.3.1/ -lgcc -o kernel.elf
	arm-none-eabi-objcopy -O binary kernel.elf kernel.bin

run:
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel.bin

docs:
	naturaldocs -p ./ -i ./  -o html docs/
