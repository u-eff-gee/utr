#!/bin/bash

# Executes the getHistogram script in a loop. The code below would do
# ./getHistogram utr utr0 .root utr0.root
# ./getHistogram utr utr1 .root utr1.root
# ...

for i in `seq 0 23`
do
		./getHistogram utr utr"$i"_ .root utr"$i".root 
done
