#!/bin/bash

# loopHistogramToTxt.sh <start> <end> <prefix>
# Executes the histogramToTxt script in a loop.

if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters"
    echo "usage: $0 <start> <end> <prefix>"
    echo "This would do"
    echo "./histogramToTxt <prefix><start>.root"
    echo "..."
    echo "./histogramToTxt <prefix><end>.root"
    exit 1
fi

for i in `seq $1 $2`
do
    printf "./histogramToTxt -f $3"$i".root\n"
    ./histogramToTxt -f $3"$i".root
done
