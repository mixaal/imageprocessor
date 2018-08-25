CC=gcc
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb -fopenmp -I. -I../


OBJECTS=jpeg.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o colorize.o brush.o flip.o crop.o gradient_map.o layer_modes.o video.o
PROG=improc
FANTASY=examples/fantasy-forest
DAYNIGHT=examples/day-to-night
TRUE_BLOOD=examples/true-blood
VIDEO_PROC=examples/video-processing

LIBS=-ljpeg -lm -lavcodec -lavformat -lavutil -lpthread -fopenmp

all: $(FANTASY) $(DAYNIGHT) $(TRUE_BLOOD) $(VIDEO_PROC)

$(TRUE_BLOOD): $(TRUE_BLOOD).o $(OBJECTS)
	$(CC) -o $(TRUE_BLOOD) $(TRUE_BLOOD).o $(OBJECTS) $(LIBS)

$(DAYNIGHT): $(DAYNIGHT).o $(OBJECTS)
	$(CC) -o $(DAYNIGHT) $(DAYNIGHT).o $(OBJECTS) $(LIBS)

$(FANTASY): $(FANTASY).o $(OBJECTS)
	$(CC) -o $(FANTASY) $(FANTASY).o $(OBJECTS) $(LIBS)

$(VIDEO_PROC): $(VIDEO_PROC).o $(OBJECTS)
	$(CC) -o $(VIDEO_PROC) $(VIDEO_PROC).o $(OBJECTS) $(LIBS)

$(PROG): main.o $(OBJECTS)
	$(CC) -o $(PROG) main.o $(OBJECTS) $(LIBS)

video.o: video.c
	$(CC) -Wall -O2 -ggdb -c $< -o $@

clean:
	$(RM) $(PROG) $(VIDEO_PROC) $(TRUE_BLOOD) $(DAYNIGHT) $(FANTASY) main.o $(VIDEO_PROC).o $(PROG).o $(TRUE_BLOOD).o $(DAYNIGHT).o $(FANTASY).o $(OBJECTS)
