for t in 1 2 3 4 5 6 7 8
do
  for n in 1 10 100 1000 10000 100000 500000 1000000
  do
        clang++ -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/opt/libomp/lib -lomp proj1.cpp -DNUMT=$t -DNUMTRIALS=$n -o proj1
    ./proj1
  done
done