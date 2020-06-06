linux:
	gcc -c -Wall -O2 math.c -o math.o	
	gcc -c -Wall -O2 rawtex.c -o rawtex.o 
	gcc -c -Wall -O2 md2.c -o md2.o
	gcc -c -Wall -O2 utgatex.c -o utgatex.o
	gcc -c -Wall -O2 models.c -o models.o 
	gcc -c -Wall -O2 map.c -o map.o
	gcc -c -Wall -O2 ctgatex.c -o ctgatex.o
	gcc -c -Wall -O2 colision.c -o colision.o
	gcc -c -Wall -O2 part.c -o part.o
	gcc -c -Wall -O2 ai.c -o ai.o
	gcc -Wall -O2 colision.o rawtex.o ai.o math.c md2.o part.o utgatex.o ctgatex.o map.o models.o base.c -o sbaloon -lSDL -lGL -lGLU -lSDL_mixer -lm

cygwin:
	gcc -c -Wall -O2 math.c -o math.o	
	gcc -c -Wall -O2 rawtex.c -o rawtex.o 
	gcc -c -Wall -O2 md2.c -o md2.o
	gcc -c -Wall -O2 utgatex.c -o utgatex.o
	gcc -c -Wall -O2 models.c -o models.o 
	gcc -c -Wall -O2 map.c -o map.o
	gcc -c -Wall -O2 ctgatex.c -o ctgatex.o
	gcc -c -Wall -O2 colision.c -o colision.o
	gcc -c -Wall -O2 part.c -o part.o
	gcc -c -Wall -O2 ai.c -o ai.o
	gcc -Wall -O2 colision.o rawtex.o ai.o math.c md2.o part.o utgatex.o ctgatex.o map.o models.o base.c -o sbaloon.exe -lopengl32 -lglu32 -lSDL -lSDL_mixer -lm


utils:
	gcc -o comp -O2 compile.c
	gcc -o conv -O2 converter.c

clean:
	rm -rf *.o sbaloon comp conv
