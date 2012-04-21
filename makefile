ifdef SystemRoot
	RM = del /Q
	LIBS = -lmingw32 -lSDLmain -lSDL -lSDL_image
else
	RM = rm -f
	LIBS = -lSDLmain -lSDL -lSDL_image
endif

CC=g++

OBJ = main.o base-classes.o monsters.o tools.o

all: TheEngine

main.o: main.cpp init.hxx base-classes.hxx monsters.hxx tools.hxx os.hxx
	$(CC) -c main.cpp

base-classes.o: base-classes.cpp base-classes.hxx tools.hxx os.hxx
	$(CC) -c base-classes.cpp

monsters.o: monsters.cpp monsters.hxx base-classes.hxx os.hxx
	$(CC) -c monsters.cpp

tools.o: tools.cpp tools.hxx os.hxx
	$(CC) -c tools.cpp

TheEngine: $(OBJ)
	$(CC) -Wall -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	$(RM) *.o 
