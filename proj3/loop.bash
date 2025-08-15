#!/bin/bash
for n in 16384 32768 64000 128000
do
  for m in true true true true true false false false false false false false false false false
  do
       clang++ -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/Cellar/libomp/16.0.0/lib -lomp -DNUMN=$n -DUSE_MUTEX=$m mutex03.cpp -o mutex03
      ./mutex03
  done
done