CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3
ROOTFLAGS=`root-config --cflags --glibs`

all: getHistogram histogramToTxt rootToTxt angdisttest

getHistogram: GetHistogram.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..
	cp loopGetHistogram.sh ..

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..
	cp loopHistogramToTxt.sh ..

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..

angdisttest: AngularDistributionGenerator_Test.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..

.PHONY: all clean

clean:
	rm getHistogram
	rm ../getHistogram
	rm loopGetHistogram
	rm ../loopGetHistogram.sh
	rm histogramToTxt
	rm ../histogramToTxt
	rm angdisttest
	rm ../angdisttest
	rm loopHistogramToTxt
	rm ../loopHistogramToTxt.sh
