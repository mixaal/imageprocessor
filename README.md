# imageprocessor
Process images in a batch way. Implement various filter for multiple image and video processing.

## Latest Build

[![Build Status](https://travis-ci.com/mixaal/imageprocessor.svg?branch=master)](https://travis-ci.com/mixaal/imageprocessor)

## Build

```
# Download all dependencies
./autogen.sh 
# Download opencv4
./get.opencv4
# Install opencv4
./install.opencv4
# Switch to cv
workon cv # On mac-os run bash to source the environment
# Generate configure file
./auto-gen.sh
# Check installed packages
./configure
# Build the source
make
```

## Examples

For further information [see the following articles](articles)

## Vectorscope Diagram

```c
     layer_t vectorscope_layer = layer_new_dim(400, 400, layer.color_components, False, False);
     vectorscope_t out;
     int max_H, max_S;
     vectorscope(layer,  out, &max_H, &max_S, layer.zone);
     draw_vectorscope(vectorscope_layer, out);
     write_JPEG_file("vectorscope.jpg", vectorscope_layer, 90);

```
![Vectorscope example](data/vectorscope.jpg)
