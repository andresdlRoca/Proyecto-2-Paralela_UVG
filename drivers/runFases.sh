#!/bin/bash

# Compilar el programa con mpicc
mpicc -o fases fases.c -lcrypto -lssl -Wno-deprecated-declarations

# Ejecutar el programa con mpirun usando 4 procesos
mpirun -np 4 fases
