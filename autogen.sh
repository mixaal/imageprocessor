#!/bin/bash -xe

OS=$(uname -s)

[ "$OS" = "Linux" ] && {
  sudo apt install -y libjpeg-dev
  sudo apt install -y libexif-dev
  sudo apt install -y ffmpeg
  sudo apt install -y automake
  sudo apt install -y cmake
  sudo apt install -y pkg-config
  sudo apt install -y tesseract-ocr
  #sudo apt install -y libblas-dev
  sudo apt install -y libopenblas-dev
  sudo apt install -y python-dev

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
 brew install automake
 brew install ffmpeg
 brew install pkg-config
 brew install wget
 brew install cmake
 brew install tesseract
}
