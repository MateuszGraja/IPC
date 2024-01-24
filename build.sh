#!/bin/bash

gcc -c common.c inf155901_155859_k.c inf155901_155859_s.c

gcc common.o inf155901_155859_k.o -o out/inf155901_155859_k
gcc common.o inf155901_155859_s.o -o out/inf155901_155859_s

rm common.o inf155901_155859_k.o inf155901_155859_s.o