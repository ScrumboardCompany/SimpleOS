TARGET = SimpleOS
AC = i686-elf-as
LC = i686-elf-gcc
CPPC = i686-elf-g++

CPPSRC = $(wildcard src/**/*.cpp)
CPPOBJ = $(patsubst %.cpp, %.o, $(CPPSRC))

ASRC = $(wildcard *.s)
AOBJ = $(patsubst %.s, %.o, $(ASRC))

$(TARGET) : $(CPPOBJ) $(AOBJ)
	$(LC) -T linker.ld -o iso/boot/$(TARGET).bin -ffreestanding -O2 -nostdlib $(AOBJ) $(CPPOBJ) -lgcc

%.o : %.s
	$(AC) $< -o $@

%.o : %.cpp 
	$(CPPC) -c $< -o $@ -Iinclude -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

clean :
	del $(TARGET) *.o