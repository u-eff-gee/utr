CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -I$(INCLUDE_DIR)
ROOTFLAGS=`root-config --cflags --glibs`

all: getHistogram histogramToTxt rootToTxt

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

.PHONY: all clean

clean:
	rm getHistogram
	rm ../getHistogram
	rm ../loopGetHistogram.sh
	rm histogramToTxt
	rm ../histogramToTxt
	rm ../loopHistogramToTxt.sh
