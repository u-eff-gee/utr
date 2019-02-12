#!/bin/bash
if [ -e "master.root" ]; then
	for i in *.root
	do
		if [ -e $i ]; then 
		rm $i
		fi
	done
	echo "All ROOT files removed."
else
	echo "No ROOT files found."
fi

