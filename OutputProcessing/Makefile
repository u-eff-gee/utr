CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -std=c++11
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree

all: getHistogram getSolidAngleCoverage histogramToTxt rootToTxt mergeFiles

getHistogram: GetHistogram.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

getSolidAngleCoverage: GetSolidAngleCoverage.cpp
	$(CPP) -o $@ $< $(CFLAGS) -std=c++17 $(ROOTFLAGS)

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

mergeFiles: MergeFiles.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

.PHONY: all clean

clean:
	for f in getHistogram getHistogramRDF histogramToTxt mergeFiles rootToTxt ; do \
	  if [ -e "$$f" ] ; then \
	    rm "$$f" ; \
	  fi ; \
	done
