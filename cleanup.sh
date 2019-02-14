#!/bin/bash
b=false
for i in *
 do
 	j=$(echo $i | tail -c 6)
 	k=$(echo $i | head -c 3)
 	if [ ".root" == "${j}" ];then
 		b=true
 		if [ "utr" == "${k}" ];then
 			rm $i
 		fi
 		if [ -e  "master.root" ];then
 			rm master.root
 		fi
 	fi
 done
if [ "$b" == "true" ];then
	echo "All ROOT files from simulation removed."
else
	echo "No ROOT files from simulation found."
fi
