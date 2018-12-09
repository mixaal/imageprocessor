#!/bin/bash -xe

sudo apt-get install -y automake autoconf libtool libjpeg-dev libavformat-dev libavcodec-dev libavutil-dev

aclocal
autoconf
autoreconf -ifv
