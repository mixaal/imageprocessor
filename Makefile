CC=gcc
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb -I. -I../


OBJECTS=jpeg.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o colorize.o brush.o flip.o crop.o
PROG=improc
FANTASY=examples/fantasy-forest

LIBS=-ljpeg -lm

$(FANTASY): $(FANTASY).o $(OBJECTS)
	$(CC) -o $(FANTASY) $(FANTASY).o $(OBJECTS) $(LIBS)

$(PROG): main.o $(OBJECTS)
	$(CC) -o $(PROG) main.o $(OBJECTS) $(LIBS)

clean:
	$(RM) $(PROG) $(FANTASY) main.o $(PROG).o $(FANTASY).o $(OBJECTS)
