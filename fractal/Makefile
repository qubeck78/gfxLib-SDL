#
# Makefile for WhiteDragon user program
#

TARGET = fractal
#CC = x86_64-w64-mingw32-gcc -c -O3 -fpermissive -w -Wl,-subsystem,windows 
CC = x86_64-w64-mingw32-g++ -c -O3 -fpermissive  

LD = x86_64-w64-mingw32-g++ 

#OBJCOPY = riscv-none-embed-objcopy
#OBJDUMP = riscv-none-embed-objdump 


all: $(TARGET).exe

$(TARGET).exe: main.o bsp.o osAlloc.o osFile.o gfBitmap.o gfDrawing.o gfFont.o osUIEvents.o
	$(LD) main.o ../gfxLib/bsp.o ../gfxLib/osAlloc.o ../gfxLib/osFile.o ../gfxLib/gfBitmap.o ../gfxLib/gfDrawing.o ../gfxLib/gfFont.o ../gfxLib/osUIEvents.o -lmingw32  -lSDL2main -lSDL2 -o $(TARGET)


main.o: main.cpp
	$(CC) -o main.o main.cpp 

bsp.o: ../gfxLib/bsp.cpp
	$(CC) -o ../gfxLib/bsp.o ../gfxLib/bsp.cpp 


gfBitmap.o: ../gfxLib/gfBitmap.cpp
	$(CC) -o ../gfxLib/gfBitmap.o ../gfxLib/gfBitmap.cpp 
	
gfDrawing.o: ../gfxLib/gfDrawing.cpp
	$(CC) -o ../gfxLib/gfDrawing.o ../gfxLib/gfDrawing.cpp 

gfGouraud.o: ../gfxLib/gfGouraud.cpp
	$(CC) -o ../gfxLib/gfGouraud.o ../gfxLib/gfGouraud.cpp 

gfFont.o: ../gfxLib/gfFont.cpp
	$(CC) -o ../gfxLib/gfFont.o ../gfxLib/gfFont.cpp 
	
osAlloc.o: ../gfxLib/osAlloc.cpp
	$(CC) -o ../gfxLib/osAlloc.o ../gfxLib/osAlloc.cpp 

osFile.o: ../gfxLib/osFile.cpp
	$(CC) -o ../gfxLib/osFile.o ../gfxLib/osFile.cpp 

osUIEvents.o: ../gfxLib/osUIEvents.cpp
	$(CC) -o ../gfxLib/osUIEvents.o ../gfxLib/osUIEvents.cpp 

clean:
	rm -f *.o *.rec *.disasm *.elf *.mif *.mi *.bin *.map *.exe
	rm -f ../gfxLib/*.o

run: $(TARGET).exe
	$(TARGET).exe
	
	