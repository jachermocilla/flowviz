OBJECTS= flowviz.o io.o utils.o d8.o view3d.o nehe.o

CC= gcc

all: flowviz

%.o: %.c
	$(CC) `sdl-config --cflags --libs`  -Os -Wall -pedantic -ffloat-store -c -g -o  $@ $<

clean:
	rm -f *.o;rm flowviz

flowviz: $(OBJECTS)
	$(CC) -g -o flowviz $(OBJECTS) -L/usr/X11R6/lib -lm -lGL -lGLU -lXxf86vm `sdl-config --cflags --libs`
