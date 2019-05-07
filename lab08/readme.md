### How to compile

search_max lib

```bash
cd search_max
make
```

fib and simd component 

```bash
gcc simd/simd.c -fopenmp -lm -o bin/simd.o
gcc fib/fib.c -fopenmp -lm -o bin/fib.o
```