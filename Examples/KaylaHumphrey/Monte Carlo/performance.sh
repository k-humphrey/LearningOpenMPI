#! /bin/bash
echo Begining performance evaluation...
mpicc monteCarloNB.c -o monteNB
MAX=9223372036854775807
intPoints=1024

while (( intPoints <= MAX / 2 )); do
    echo "Running with input: $intPoints"
    mpirun ./monteB "$intPoints"
    intPoints=$((intPoints * 2))
    sleep 3
done