CC=gcc
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb


OBJECTS=jpeg.o main.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o
PROG=improc

LIBS=-ljpeg -lm

$(PROG): $(OBJECTS)
	$(CC) -o $(PROG) $(OBJECTS) $(LIBS)

clean:
	$(RM) $(PROG) $(OBJECTS)
