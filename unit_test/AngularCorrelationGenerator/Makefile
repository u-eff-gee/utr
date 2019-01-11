CPP=g++
SRC_DIR=../../src
INCLUDE_DIR=../../include
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -I$(INCLUDE_DIR)
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree

all: angcorrtest

AngularDistribution.o: $(SRC_DIR)/AngularDistribution.cc $(INCLUDE_DIR)/AngularDistribution.hh
	$(CPP) -c -o $@ $< $(CFLAGS)

angcorrtest: AngularDistribution.o AngularCorrelationGenerator_Test.cpp
	$(CPP) -o $@ $^ $(CFLAGS) $(ROOTFLAGS)
	cp $@ ../../

.PHONY: all clean

clean:
	rm angcorrtest
	rm AngularDistribution.o
	rm ../../angcorrtest
