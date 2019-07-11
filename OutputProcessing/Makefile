CPP=g++
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree

all: getHistogram histogramToTxt rootToTxt mergeFiles fep_efficiency

getHistogram: GetHistogram.cpp getHistogramRDF.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

histogramToTxt: HistogramToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

mergeFiles: MergeFiles.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

rootToTxt: RootToTxt.cpp
	$(CPP) -o $@ $< $(CFLAGS) $(ROOTFLAGS)

fep_efficiency: fep_efficiency.sh

.PHONY: all clean

clean:
	for f in getHistogram getHistogramRDF histogramToTxt mergeFiles rootToTxt ; do \
	  if [ -e "$$f" ] ; then \
	    rm "$$f" ; \
	  fi ; \
	done
