CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree

all: getHistogram histogramToTxt rootToTxt mergeFiles fep_efficiency

getHistogram: GetHistogram.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..
	cp loopGetHistogram.sh ..

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..
	cp loopHistogramToTxt.sh ..

mergeFiles: MergeFiles.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)
	cp $@ ..

fep_efficiency: fep_efficiency.sh
	cp $< ..

.PHONY: all clean

clean:
	rm getHistogram
	rm ../getHistogram
	rm ../loopGetHistogram.sh
	rm histogramToTxt
	rm ../histogramToTxt
	rm ../loopHistogramToTxt.sh
	rm mergeFiles
	rm ../mergeFiles
	rm rootToTxt
	rm ../rootToTxt
	rm ../fep_efficiency.sh
