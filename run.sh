#! /bin/bash
mpicc -o sample sample.c
mpirun -np 10 ./sample -fname nice
