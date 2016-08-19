#!/bin/bash
for i in `seq 1 1000`;
do
    ./mincut.py kargerMinCut.txt >> results.txt
done

