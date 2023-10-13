#!/bin/bash

# Compilar el programa con mpicc
mpicc -o bf2 bf2.c -lcrypto -lssl -Wno-deprecated-declarations

# Ejecutar el programa con mpirun usando 4 procesos
mpirun -np 4 bf2
