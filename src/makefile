ifdef SystemRoot
	RM = del /Q
	LIBS = -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf
	FLAGS = -mwindows
	BIN = ../bin/TheEngine.exe
else
	RM = rm -f
	LIBS = -lSDLmain -lSDL -lSDL_image -lSDL_ttf
	FLAGS = 
	BIN = ../bin/TheEngine
endif

CC = g++

OBJ = ../bin/main.o ../bin/base-classes.o ../bin/monsters.o ../bin/tools.o

all: TheEngine

../bin/main.o: main.cpp init.hxx base-classes.hxx monsters.hxx tools.hxx os.hxx
	$(CC) -c main.cpp -o ../bin/main.o

../bin/base-classes.o: base-classes.cpp base-classes.hxx tools.hxx os.hxx
	$(CC) -c base-classes.cpp -o ../bin/base-classes.o

../bin/monsters.o: monsters.cpp monsters.hxx base-classes.hxx os.hxx
	$(CC) -c monsters.cpp -o ../bin/monsters.o

../bin/tools.o: tools.cpp tools.hxx os.hxx
	$(CC) -c tools.cpp -o ../bin/tools.o

TheEngine: $(OBJ)
	$(CC) -Wall -o "../bin/$@" $^ $(LIBS) $(FLAGS)

.PHONY: clean

clean:
	$(RM) "$(BIN)" "../bin/*.o" "../bin/stdout.txt" "../bin/stderr.txt"
