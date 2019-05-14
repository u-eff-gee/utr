#!/bin/bash

# loopGetHistogram.sh <start> <end> <treename> <prefix> [<bin>]
# Executes the getHistogram script in a loop.

if [ "$#" -lt 4 ]; then
    echo "Illegal number of parameters"
    echo "usage: $0 <start> <end> <treename> <prefix> [<bin>]"
    echo "This would do"
    echo "./getHistogram -t <treename> -p <prefix><start>_ -q .root -o <prefix><start>.root [-b <bin>]"
    echo "..."
    echo "./getHistogram -t <treename> -p <prefix><end>_ -q .root -o <prefix><end>.root [-b <bin>]"
    exit 1
fi

if [ "$#" -eq 5 ]; then
	for i in `seq $1 $2`
	do
#	    echo ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root -b $5
	    ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root -b $5
	done
else
	for i in `seq $1 $2`
	do
	    echo ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root
	    ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root
	done
fi
