CC=gcc
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb -I. -I../


OBJECTS=jpeg.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o colorize.o brush.o flip.o crop.o gradient_map.o layer_modes.o
PROG=improc
FANTASY=examples/fantasy-forest
DAYNIGHT=examples/day-to-night

LIBS=-ljpeg -lm

all: $(FANTASY) $(DAYNIGHT)

$(DAYNIGHT): $(DAYNIGHT).o $(OBJECTS)
	$(CC) -o $(DAYNIGHT) $(DAYNIGHT).o $(OBJECTS) $(LIBS)

$(FANTASY): $(FANTASY).o $(OBJECTS)
	$(CC) -o $(FANTASY) $(FANTASY).o $(OBJECTS) $(LIBS)

$(PROG): main.o $(OBJECTS)
	$(CC) -o $(PROG) main.o $(OBJECTS) $(LIBS)

clean:
	$(RM) $(PROG) $(DAYNIGHT) $(FANTASY) main.o $(PROG).o $(DAYNIGHT).o $(FANTASY).o $(OBJECTS)
