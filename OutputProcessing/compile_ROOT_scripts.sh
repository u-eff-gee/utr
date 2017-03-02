#!/bin/bash

# GetHistogram.cpp
g++ -o getHistogram GetHistogram.cpp `root-config --cflags --glibs`
cp getHistogram ..
cp loopGetHistogram.sh ..

# RootToTxt.cpp
cp RootToTxt.cpp ..
