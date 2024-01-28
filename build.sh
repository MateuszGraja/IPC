#!/bin/bash

# -Werror
gcc -Wall -c src/utils/*.c src/*.c

# Potrzebujemy wszystkich składowych plików oprócz klienta w serwerze i odwrotnie 
gcc $(ls *.o | grep -v inf155901_155859_s.o) -o out/inf155901_155859_k
gcc $(ls *.o | grep -v inf155901_155859_k.o) -o out/inf155901_155859_s

rm *.o