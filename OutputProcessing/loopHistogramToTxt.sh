#!/bin/bash

# Executes the histogramToTxt script in a loop. The code below would do
# ./histogramToTxt utr0.root
# ./histogramToTxt utr1.root

for i in `seq 0 1`
do
	./histogramToTxt utr"$i".root
done
