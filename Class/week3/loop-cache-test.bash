for n in 10 50 100 150 200 250 300 350 400 450 1000 1500 2000 3000 5000 10000 15000 20000
  do
    clang++ -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/opt/libomp/lib -lomp -DSIZE=$n cache-test.cpp -o cache-test ./cache-test.cpp
  done
