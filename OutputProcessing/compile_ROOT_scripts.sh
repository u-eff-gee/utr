#!/bin/bash

# GetHistogram.cpp
g++ -o getHistogram GetHistogram.cpp `root-config --cflags --glibs`
cp getHistogram ..
cp loopGetHistogram.sh ..

# HistogramToTxt.cpp
cp HistogramToTxt.cpp ..

# RootToTxt.cpp
cp RootToTxt.cpp ..
