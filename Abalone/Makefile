CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lSDL2main -lSDL2 -lSDL2_image -lm -lpthread
EXEC=Abalone

all: $(EXEC)

$(EXEC): AbaloneBoard.o Button.o Event.o EventManager.o main.o minimax.o MinimaxThreaded.o SelectBoardInterface.o SelectPlayerInterface.o TextureManager.o WindowManager.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
