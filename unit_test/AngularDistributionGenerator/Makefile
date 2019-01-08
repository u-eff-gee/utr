CPP=g++
SRC_DIR=../../src
INCLUDE_DIR=../../include
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -I$(INCLUDE_DIR)
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree

all: angdisttest

AngularDistribution.o: $(SRC_DIR)/AngularDistribution.cc $(INCLUDE_DIR)/AngularDistribution.hh
	$(CPP) -c -o $@ $< $(CFLAGS)

angdisttest: AngularDistribution.o AngularDistributionGenerator_Test.cpp
	$(CPP) -o $@ $^ $(CFLAGS) $(ROOTFLAGS)
	cp $@ ../../

.PHONY: all clean

clean:
	rm angdisttest
	rm AngularDistribution.o
	rm ../../angdisttest
