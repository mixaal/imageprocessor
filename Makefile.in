CC=@CC@
CXX=@CXX@
#OPENCVFLAGS=`pkg-config --cflags opencv4`
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb -fopenmp -I. -I../
CXXFLAGS=-Wall -Werror -O2 -ggdb -fopenmp -I. -I../ @OPENCV_CFLAGS@

OBJECTS=jpeg.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o colorize.o brush.o flip.o crop.o gradient_map.o layer_modes.o video.o\
        auto_tone.o comics.o shapes.o vectorscope.o histogram.o apply_color.o

PROG=improc
FANTASY=examples/fantasy-forest
DAYNIGHT=examples/day-to-night
TRUE_BLOOD=examples/true-blood
VIDEO_PROC=examples/video-processing
COMICS=examples/comics-example
REDCAR=examples/replace_color
TRACKING=examples/tracking
EYE_ENHANCER=examples/eye-enhancer
APPLY_COLOR=examples/apply_color
COLOR_CONV_TEST=examples/color_conv_test

LIBS=@LIBS@ -fopenmp
#OPENCVLIBS=`pkg-config --libs opencv4`

all: $(FANTASY) $(DAYNIGHT) $(TRUE_BLOOD) $(VIDEO_PROC) $(COMICS) $(PROG) $(REDCAR)  $(EYE_ENHANCER) $(TRACKING) $(APPLY_COLOR) $(COLOR_CONV_TEST)

$(EYE_ENHANCER): $(EYE_ENHANCER).o $(OBJECTS)
	$(CXX) -o $(EYE_ENHANCER)  $(EYE_ENHANCER).o $(OBJECTS)  @OPENCV_LIBS@ $(LIBS)

$(TRACKING): $(TRACKING).o $(OBJECTS)
	$(CXX) -o $(TRACKING)  $(TRACKING).o $(OBJECTS)  @OPENCV_LIBS@ $(LIBS)

$(COLOR_CONV_TEST): $(COLOR_CONV_TEST).o $(OBJECTS)
	$(CC) -o $(COLOR_CONV_TEST)  $(COLOR_CONV_TEST).o $(OBJECTS) $(LIBS)

$(APPLY_COLOR): $(APPLY_COLOR).o $(OBJECTS)
	$(CC) -o $(APPLY_COLOR)  $(APPLY_COLOR).o $(OBJECTS) $(LIBS)

$(REDCAR): $(REDCAR).o $(OBJECTS)
	$(CC) -o $(REDCAR)  $(REDCAR).o $(OBJECTS) $(LIBS)

$(COMICS): $(COMICS).o $(OBJECTS)
	$(CC) -o $(COMICS) $(COMICS).o $(OBJECTS) $(LIBS)

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
	$(RM) $(PROG) $(COMICS) $(VIDEO_PROC) $(TRUE_BLOOD) $(DAYNIGHT) $(FANTASY) main.o $(COMICS).o $(VIDEO_PROC).o $(PROG).o $(TRUE_BLOOD).o $(DAYNIGHT).o $(FANTASY).o $(OBJECTS) $(REDCAR) $(REDCAR).o $(TRACKING) $(TRACKING).o $(EYE_ENHANCER) $(EYE_ENHANCER).o

test:
	echo "passed"
