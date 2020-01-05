CC=@CC@
CXX=@CXX@
AR=ar
RANLIB=@RANLIB@
#OPENCVFLAGS=`pkg-config --cflags opencv4`
RM=rm -f
CFLAGS=-Wall -Werror -O2 -ggdb -I. -I../ -fPIC
CXXFLAGS=-Wall -Werror -O2 -ggdb -I. -I../ -std=c++11 @OPENCV_CFLAGS@

OBJECTS=jpeg.o xmalloc.o bw.o gauss.o layer.o unsharp.o contrast.o brightness.o gamma.o invert.o \
        temperature.o tint.o exposure.o color_conversion.o common.o kelvin_temp.o color_balance.o \
        saturation.o vibrance.o colorize.o brush.o flip.o crop.o gradient_map.o layer_modes.o video.o\
        auto_tone.o comics.o shapes.o vectorscope.o histogram.o apply_color.o reinhard2001.o llist.o kmeans.o\
        dong2010.o permgen.o emd.o lut.o exif.o sincity.o levels.o laplace.o median.o

PROG=improc
LUT_MAIN=examples/apply_lut
DOMINANT_COLORS=examples/dominant_colors
FANTASY=examples/fantasy-forest
DAYNIGHT=examples/day-to-night
TRUE_BLOOD=examples/true-blood
VIDEO_PROC=examples/video-processing
COMICS=examples/comics-example
REDCAR=examples/replace_color
TRACKING=examples/tracking
EYE_ENHANCER=examples/eye-enhancer
APPLY_COLOR=examples/apply_color
APPLY_COLOR_REINHARD=examples/apply_color_reinhard2001
APPLY_COLOR_DONG=examples/apply_color_dong2010
COLOR_CONV_TEST=examples/color_conv_test
SINCITY=examples/sincity
LEVELS=examples/levels
LAPLACE=examples/laplace
MEDIAN=examples/median
LIBIMAGEPROCESSOR=libimageprocessor.a
LIB_SHARED_IMAGE_PROCESSOR=libimageprocessor.so

SUFFIXES=.o .c .cc .cpp

LIBS=@LIBS@ @OPENMP_CFLAGS@
#OPENCVLIBS=`pkg-config --libs opencv4`

all: $(LUT_MAIN) $(FANTASY) $(DAYNIGHT) $(TRUE_BLOOD) $(VIDEO_PROC) $(COMICS) $(PROG) $(REDCAR)  $(TRACKING) $(APPLY_COLOR) $(COLOR_CONV_TEST) $(APPLY_COLOR_REINHARD) $(DOMINANT_COLORS) $(APPLY_COLOR_DONG) $(SINCITY) $(LEVELS) $(LAPLACE) $(LIBIMAGEPROCESSOR) $(LIB_SHARED_IMAGE_PROCESSOR) $(MEDIAN)

$(LIB_SHARED_IMAGE_PROCESSOR): $(OBJECTS)
	$(CC) -shared -o $(LIB_SHARED_IMAGE_PROCESSOR) $(OBJECTS) $(LIBS)

$(LIBIMAGEPROCESSOR): $(OBJECTS)
	$(AR) rcs $(LIBIMAGEPROCESSOR) $(OBJECTS)

$(MEDIAN): $(MEDIAN).o $(OBJECTS)
	$(CC) -o $(MEDIAN)  $(MEDIAN).o $(OBJECTS) $(LIBS)

$(LAPLACE): $(LAPLACE).o $(OBJECTS)
	$(CC) -o $(LAPLACE)  $(LAPLACE).o $(OBJECTS) $(LIBS)

$(LEVELS): $(LEVELS).o $(OBJECTS)
	$(CC) -o $(LEVELS)  $(LEVELS).o $(OBJECTS) $(LIBS)

$(SINCITY): $(SINCITY).o $(OBJECTS)
	$(CC) -o $(SINCITY)  $(SINCITY).o $(OBJECTS) $(LIBS)

$(EYE_ENHANCER): $(EYE_ENHANCER).o $(OBJECTS)
	$(CXX) -o $(EYE_ENHANCER)  $(EYE_ENHANCER).o $(OBJECTS)  @OPENCV_LIBS@ $(LIBS)

$(TRACKING): $(TRACKING).o $(OBJECTS)
	$(CXX) -o $(TRACKING)  $(TRACKING).o $(OBJECTS)  @OPENCV_LIBS@ $(LIBS)

$(DOMINANT_COLORS): $(DOMINANT_COLORS).o $(OBJECTS)
	$(CC) -o $(DOMINANT_COLORS)  $(DOMINANT_COLORS).o $(OBJECTS) $(LIBS)

$(COLOR_CONV_TEST): $(COLOR_CONV_TEST).o $(OBJECTS)
	$(CC) -o $(COLOR_CONV_TEST)  $(COLOR_CONV_TEST).o $(OBJECTS) $(LIBS)


$(APPLY_COLOR_REINHARD): $(APPLY_COLOR_REINHARD).o $(OBJECTS)
	$(CC) -o $(APPLY_COLOR_REINHARD)  $(APPLY_COLOR_REINHARD).o $(OBJECTS) $(LIBS)

$(APPLY_COLOR_DONG): $(APPLY_COLOR_DONG).o $(OBJECTS)
	$(CC) -o $(APPLY_COLOR_DONG)  $(APPLY_COLOR_DONG).o $(OBJECTS) $(LIBS)

$(LUT_MAIN): $(LUT_MAIN).o $(OBJECTS)
	$(CC) -o $(LUT_MAIN)  $(LUT_MAIN).o $(OBJECTS) $(LIBS)

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
	$(RM) $(PROG) $(COMICS) $(VIDEO_PROC) $(TRUE_BLOOD) $(DAYNIGHT) $(FANTASY) main.o $(COMICS).o $(VIDEO_PROC).o $(PROG).o $(TRUE_BLOOD).o $(DAYNIGHT).o $(FANTASY).o $(OBJECTS) $(REDCAR) $(REDCAR).o $(TRACKING) $(TRACKING).o $(EYE_ENHANCER) $(EYE_ENHANCER).o $(DOMINANT_COLORS) $(DOMINANT_COLORS).o $(APPLY_COLOR_DONG).o $(APPLY_COLOR_DONG) $(APPLY_COLOR_REINHARD).o $(APPLY_COLOR_REINHARD) $(APPLY_COLOR).o $(APPLY_COLOR) $(COLOR_CONV_TEST).o $(COLOR_CONV_TEST) $(LUT_MAIN) $(LUT_MAIN).o $(LIBIMAGEPROCESSOR) $(LIB_SHARED_IMAGE_PROCESSOR) $(LEVELS) $(LAPLACE) $(SINCITY) $(COMICS) $(LEVELS).o $(LAPLACE).o $(MEDIAN) $(MEDIAN).o

test:
	echo "passed"
