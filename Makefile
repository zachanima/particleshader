PROJECT = particleshader
IN      = src/*.cpp src/**/*.cpp
OUT     = bin/$(PROJECT)

CC      = g++
CFLAGS  = -O2 -ansi -pedantic -g -Wall
LFLAGS  = -lGL -lSDL -lGLEW

all: $(PROJECT)

$(PROJECT): $(IN)
	$(CC) $(CFLAGS) $(IN) -o "$(OUT)" $(LFLAGS)

