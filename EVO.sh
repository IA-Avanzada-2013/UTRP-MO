#!/bin/bash

cnf_filename=$1
instance_specifics=$2
cutoff_time=$3
cutoff_length=$4
seed=$5

shift 5

output="resultados.txt"
rm -rf ${output}

me=100000

pc=0.5
pm=0.5
ps=100
fx=1000

while [ $# != 0 ]; do
    flag="$1"
    case "$flag" in
        -pc) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              pc=$arg
            fi
            ;;
        -pm) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              pm=$arg
            fi
            ;;
        -ps) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              ps=$arg
            fi
            ;;
        -fx) if [ $# -gt 1 ]; then
              arg="$2"
              shift
              fx=$arg
            fi
            ;;
        *) echo "Unrecognized flag or argument: $flag"
            ;;
        esac
    shift
done

#linea de codigo a especificar 
#echo "./ga-nk ${cnf_filename} ${output} ${cr} ${mr} ${ps} ${me} ${seed}"
echo "./utrpmo -i instances/Mandl -s ${seed} -r "6:2:8" -p ${ps}:${pm}:${pc}:${fx}"
./utrpmo -i instances/Mandl -s ${seed} -r "6:2:8" -p ${ps}:${pm}:${pc}:${fx}
#./utrpmo -i instances/Mumford0 -s ${seed} -r "12:2:15" -p ${ps}:${pm}:${pc}:${fx}
#./utrpmo -i instances/Mumford1 -s ${seed} -r "15:10:30" -p ${ps}:${pm}:${pc}:${fx}
#./utrpmo -i instances/Mumford2 -s ${seed} -r "56:10:22" -p ${ps}:${pm}:${pc}:${fx}
#./utrpmo -i instances/Mumford3 -s ${seed} -r "60:12:25" -p ${ps}:${pm}:${pc}:${fx}

solved="SAT"
runlength=`cat ${output}`
runtime=0
best_sol=0

echo "Result for ParamILS: ${solved}, ${runtime}, ${runlength}, ${best_sol}, ${seed}"
