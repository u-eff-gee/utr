#!/bin/bash

# loopHistogramToTxt.sh <start> <end> <prefix>
# Executes the histogramToTxt script in a loop.

if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters"
    echo "usage: $0 <start> <end> <prefix>"
    echo "This would do"
    echo "../OutputProcessing/histogramToTxt <prefix><start>.root"
    echo "..."
    echo "../OutputProcessing/histogramToTxt <prefix><end>.root"
    exit 1
fi

for i in `seq $1 $2`
do
    echo ../OutputProcessing/histogramToTxt $3"$i".root
    ../OutputProcessing/histogramToTxt $3"$i".root
done
