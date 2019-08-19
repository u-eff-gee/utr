if [ "$#" -lt 5 ]; then
	echo "Usage:
$(basename "$0") PREFIX START STOP SUFFIX NSIM
The script loops over text histograms [1] with names PREFIX<i>SUFFIX (where <i> denotes all integer numbers from START to STOP, including these limits), reads out the full-energy peak (FEP) content [2] and determines the FEP efficiency.

  For example, the command
    $ ./fep_efficiency.sh det0_utr 1 3 .txt 1000000
  will loop over the following files:
    det0_utr1.txt
    det0_utr2.txt
    det0_utr3.txt

The last argument denotes the number of simulated primary particles, which is needed to calculate the efficiency. If you do not know NSIM, simply enter an arbitrary value.
Two output files will be created, where the energies and the corresponding FEP counts/efficiencies are given in the first and second column. The number of simulated primary particles will be printed in the header.

    PREFIX       File name prefix
    SUFFIX       File name suffix
    START        Begin of file numbers
    STOP         End of file numbers
    NSIM         Number of simulated primary particles

[1] A text histogram is a two-column file which contains the energy in the first column and the number of counts in the second column. Such a file is returned by the histogramToTXT script of utr, for example.
[2] The full-energy peak in the histogram file is defined as the bin with the highest energy with a nonzero content.
"
fi

FEPFILE="out_fep_"$1".txt"
EFFFILE="out_eff_"$1".txt"

if [ "$#" -eq 5 ]; then
	if test -f $FEPFILE; then
		echo "Warning: File "$FEPFILE" already existed ..."
	fi
	if test -f $EFFFILE; then
		echo "Warning: File "$EFFFILE" already existed ..."
	fi

	echo "# "$5" PRIMARY PARTICLES" > $EFFFILE
	echo "# ENERGY - EFFICIENCY" > $EFFFILE
	echo "# "$5" PRIMARY PARTICLES" > $FEPFILE
	echo "# ENERGY - FEP COUNTS" > $FEPFILE
	echo "Extracting FEP counts from the following files:"
	for i in `seq $2 $3`;
	do
		printf "\t"$1$i$4"\n"
		awk -v nsim="$5" '$2 > 0 {print $1 "\t" $2/nsim}' $1$i$4 | tail -1 >> $EFFFILE
		awk -v nsim="$5" '$2 > 0 {print $1 "\t" $2}' $1$i$4 | tail -1 >> $FEPFILE
	done
	echo "Created files "$FEPFILE" and "$EFFFILE
fi
