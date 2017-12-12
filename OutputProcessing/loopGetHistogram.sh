#!/bin/bash

# loopGetHistogram.sh <start> <end> <histname> <prefix>
# Executes the getHistogram script in a loop.

if [ "$#" -ne 4 ]; then
    echo "Illegal number of parameters"
    echo "usage: $0 <start> <end> <histname> <prefix>"
    echo "This would do"
    echo "./getHistogram <histname> <prefix><start>_ .root <prefix><start>.root"
    echo "..."
    echo "./getHistogram <histname> <prefix><end>_ .root   <prefix><end>.root"
    exit 1
fi

for i in `seq $1 $2`
do
    echo ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root
    ./getHistogram -t $3 -p $4"$i"_ -q .root -o $4"$i".root
done
