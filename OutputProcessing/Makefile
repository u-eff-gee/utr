CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3
ROOTFLAGS=`root-config --cflags --glibs`

all: getHistogram histogramToTxt rootToTxt

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

.PHONY: clean

clean:
	rm ../getHistogram
	rm ../loopGetHistogram.sh
	rm ../histogramToTxt
	rm ../loopHistogramToTxt.sh
