#!/bin/bash
for file in 000010 000100 001000 010000 100000
do
    for algo in naive regex trie
    do
        echo $algo 
        python $algo.py $file.find_replace $file-10.txt  $file-50.txt $file-100.txt
    done
done
