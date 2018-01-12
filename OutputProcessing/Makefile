CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3
ROOTFLAGS=`root-config --cflags --glibs`

all: getHistogram histogramToTxt rootToTxt angdisttest

getHistogram: GetHistogram.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	mv $@ ..
	cp loopGetHistogram.sh ..

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	mv $@ ..
	cp loopHistogramToTxt.sh ..

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	mv $@ ..

angdisttest: AngularDistributionGenerator_Test.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	mv $@ ..

.PHONY: all clean

clean:
	rm ../getHistogram
	rm ../loopGetHistogram.sh
	rm ../histogramToTxt
	rm ../angdisttest
	rm ../loopHistogramToTxt.sh
