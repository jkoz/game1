P=game1
CFLAGS = `pkg-config --cflags sdl2 SDL2_image` -D_DEFAULT_SOURCE -g -O2 -std=c99 -Wall -ansi
LDFLAGS = `pkg-config --libs sdl2 SDL2_image`
CC = gcc
OBJS = game1.o

all: $(OBJS)

game1.o: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o game1.o main.c

clean:
	rm -f game1.o game1

push:
	@if [ "x$(MSG)" = 'x' ] ; then echo "Usage: MSG='whatever.' make push"; fi
	@test "x$(MSG)" != 'x'
	git commit -a -m "$(MSG)"
	git push

pull:
	git pull
