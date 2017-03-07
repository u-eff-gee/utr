#!/bin/bash

# GetHistogram.cpp
g++ -o getHistogram GetHistogram.cpp `root-config --cflags --glibs`
cp getHistogram ..
cp loopGetHistogram.sh ..

# HistogramToTxt.cpp
g++ -o histogramToTxt HistogramToTxt.cpp `root-config --cflags --glibs`
cp histogramToTxt ..

# RootToTxt.cpp
cp RootToTxt.cpp ..
