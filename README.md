# Proyecto 2 - Computacion Paralela y Distribuida

### Prerequisitos
- Maquina Virtual (o fisica) con linux
- Compilador gcc

### Dependencias
- libreria Open MPI
- Libreria Open SSL

Instalacion de Open SSL:
1. `sudo apt update`
2. `sudo apt upgrade`
3. `sudo apt install libssl-dev`

## Programa Secuencial
Implementacion secuencial de algoritmo de Bruteforce para desencriptacion

### Compilacion

`gcc -o bfsec secuencial.c`

### Uso
`./bfsec`

## Programa Paralelo con BruteForce
Implementacion paralela de algoritmo de Bruteforce para desencriptacion

### Compilacion

`mpicc -o bf hueristic.c -lcrypto -lssl -Wno-deprecated-declarations`

### Uso

`mpirun -np 4 bruteforce`

> Nota: Se debe especificar en el archivo `./input.txt`:
> 1. La llave privada para encriptar
> 2. El mensaje para encriptar
> 3. La *llave publica* (mensaje a buscar para verificar desincriptacion)

> Ejemplo:
> 123456
>
> Esta es una prueba de proyecto 2
>
> es una prueba de

## Programa Paralelo de "Fases"
Implementacion paralela de algoritmo de Bruteforce "inteligente"
Este empieza como cualquier bruteforce, pero luego va aumentando sus rangos de prueba por un exponente
n_i ^ exp. Donde n_i es el numero de proceso y exp es el exponente.

### Compilacion

`mpicc -o fases fases.c -lcrypto -lssl -Wno-deprecated-declarations`

### Uso

`mpirun -np 4 fases`

> Nota: Se debe especificar en el archivo `./input.txt`:
> 1. La llave privada para encriptar
> 2. El mensaje para encriptar
> 3. La *llave publica* (mensaje a buscar para verificar desincriptacion)

> Ejemplo:
> 123456
>
> Esta es una prueba de proyecto 2
>
> es una prueba de

## Programa Paralelo de "bf2"


### Compilacion

`mpicc -o bf2 bf2.c -lcrypto -lssl -Wno-deprecated-declarations`

### Uso

`mpirun -np 4 bf2`

> Nota: Se debe especificar en el archivo `./input.txt`:
> 1. La llave privada para encriptar
> 2. El mensaje para encriptar
> 3. La *llave publica* (mensaje a buscar para verificar desincriptacion)

> Ejemplo:
> 123456
>
> Esta es una prueba de proyecto 2
>
> es una prueba de

## Autores
👤 Diego Cordova
- <a href= "https://github.com/Diego-CB">Github</a>

👤 Gabriela Contreras
- <a href="https://github.com/Paola-Contreras">Github</a>

👤 Andrés de la Roca  
- <a href = "https://www.linkedin.com/in/andr%C3%A8s-de-la-roca-pineda-10a40319b/">Linkedin</a>  
- <a href="https://github.com/andresdlRoca">Github</a>
