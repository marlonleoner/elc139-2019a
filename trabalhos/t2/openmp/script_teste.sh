#/bin/bash

FILE="codeOMP"

MAX_THREADS=3 # (t * 2)      => define qtdade de threads, max 4 threads
MAX_WSIZE=10  # (r * 100000) => max 1000000 repeticoes
MAX_REP=3     # (s * 1000)   => max 3000 repeticoes

SUM=0

gcc -fopenmp -o $FILE $FILE.c 

for (( r = 1; r <= $MAX_REP; r++ )) 
do
    REPET=$((r*1000))
    for (( t = 0; t < $MAX_THREADS; t++ )) 
    do
        THREAD=$((2**t))
        WORKSIZE=$(($1/THREAD))
        SUM=$((0))
        for (( re = 1; re <= 5; re++ )) 
        do
            res=`./$FILE $THREAD $WORKSIZE $REPET`
            SUM=$((SUM+res))
        done
        ( (echo -e "<$THREAD> <$WORKSIZE> <$REPET>")  && (echo -e $((SUM/5))) ) >> results.txt
    done
done

( (echo -e "<$TAM_VET> <$1>") && (./$FILE $TAM_VET $1) ) >> out.txt 