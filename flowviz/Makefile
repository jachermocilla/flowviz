OBJECTS= flowviz.o io.o utils.o d8.o nehe.o layer.o project.o elevation.o elevation_view.o deight.o rainfall.o list.o slope.o

CC= gcc
FLAGS=-lgdal1.6.0 -I/usr/include/gdal

all: flowviz

%.o: %.c
	$(CC) $(FLAGS) `sdl-config --cflags --libs`  -Os -Wall -ffloat-store -c -g -o  $@ $<

clean:
	rm -f *.o;rm flowviz

flowviz: $(OBJECTS)
	$(CC) $(FLAGS) -g -o flowviz $(OBJECTS) -L/usr/X11R6/lib -lm -lGL -lGLU -lXxf86vm `sdl-config --cflags --libs`
