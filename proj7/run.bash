
#!/bin/bash
for n in 1 2 4 8
do
g++ -DNUM_PROC=$n -o proj7 proj7.cpp -std=c++11 -lmpi
./proj7
done
