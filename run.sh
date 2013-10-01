#! /bin/bash
mpicc -o sample sample.c
mpirun -np 4 ./sample -fname nice
