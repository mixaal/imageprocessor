#!/bin/bash -xe

OS=$(uname -s)

[ "$OS" = "Linux" ] && {
  sudo apt install libjpeg-dev
  sudo apt install libexif-dev
  sudo apt install libavutil-dev libavcodec-dev
  sudo apt install libavformat-dev
  sudo apt install libavcodec-extra
  sudo apt install libopencv-dev
  sudo apt install libopencv-contrib-dev
}

[ "$OS" = "Darwin" ] && {
 brew install libjpeg 
 brew install libraw 
 brew install libexif
}
