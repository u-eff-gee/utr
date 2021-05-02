CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -std=$(shell root-config --cflags | sed 's/.*-std=\(\S\+\)\s.*/\1/')
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree -lROOTDataFrame -lTreePlayer

all: getHistogram getHistogram-Eventwise getSolidAngleCoverage histogramToTxt rootToTxt mergeFiles

getHistogram: GetHistogram.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

getHistogram-Eventwise: GetHistogram-Eventwise.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

getSolidAngleCoverage: GetSolidAngleCoverage.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

mergeFiles: MergeFiles.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

.PHONY: all clean

clean:
	for f in getHistogram getHistogram-Eventwise getHistogramRDF getSolidAngleCoverage histogramToTxt mergeFiles rootToTxt ; do \
	  if [ -e "$$f" ] ; then \
	    rm "$$f" ; \
	  fi ; \
	done
