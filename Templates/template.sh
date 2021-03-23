#!/bin/bash

function usage {
	echo
	echo  "Description of this script"
	echo
	echo -e "Usage: \033[1;31m$(basename $0)\033[0;0m"
	echo
	echo  "  -i|--input   [str] input"
	echo  "  -o|--outdir  [str] output"
	echo  "  -n|--number  [int] number"
	echo  "  -c|--clean   clean up intermediate files (default: false)"
	echo
	echo  "  -h|--help   display this message"
	echo
	echo  "Note: "
	echo
	echo  "Contact AUTHOR for more info"
	echo
	exit 1;
}

if ! options=$(getopt -o i:o:n:ch -l in:,out:,num:,clean,help -- "$@")
then
	usage;
fi

if [ $# -eq 0 ];then
	usage;
fi

set -- $options
help="no"
num=1
clean="no"

while [ $# -gt 0 ]
do
	case $1 in
		-h|--help) help="yes";;
		-i|--in) in=$(echo $2 | tr -d "'"); shift;;
		-o|--out) out=$(echo $2 | tr -d "'"); shift;;
		-n|--num) trim=$(echo $2 | tr -d "'"); shift;;
		-c|--clean) clean="yes";;
		(--) shift; break;;
		(-*) echo "$(basename $0): error - unrecognized option $1" 1>&2; exit 1;;
		(*) break;;
	esac
	shift
done

if [ $help == "yes" ];then
	usage;
	exit 1
fi

START=$(date +%s)

### check io
if [ -z $in ]; then echo -e "Input error"; exit; fi

### functions
sec2time ()
{
	T=$1
	D=$((T/60/60/24))
	H=$((T/60/60%24))
	M=$((T/60%60))
	S=$((T%60))

	if [[ ${D} != 0 ]]
	then
		printf '[TAT] %d days %02d:%02d:%02d\n' $D $H $M $S
	else
		printf '[TAT] %02d:%02d:%02d\n' $H $M $S
	fi
}

### db exports


### pragram exports


### main


### __END__
sec2time $SECONDS
