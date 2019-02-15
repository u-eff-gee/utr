#!/bin/bash
b=false
for i in *
 do
 	j=$(echo $i | tail -c 6)
 	if [ ".root" == "${j}" ];then
 		b=true
		rm $i
 	fi
 done
if [ "$b" == "true" ];then
	echo "All ROOT files removed."
else
	echo "No ROOT files found."
fi
