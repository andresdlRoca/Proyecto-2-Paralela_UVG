# Proyecto 2 - Computacion Paralela y Distribuida


## Programa Secuencial

### Prerequisitos

### Dependencias

### Compilacion

### Uso

## Programa Paralelo con BruteForce

### Prerequisitos
- Maquina Virtual (o fisica) con linux
- Compilador gcc

### Dependencias
- libreria Open MPI
- Libreria Open SSL

> Instalacion de Open SSL:
> 1. `sudo apt update`
> 2. `sudo apt upgrade`
> 3. `sudo apt install libssl-dev`

### Compilacion

`mpicc -o bruteforce bruteforce.c -lcrypto -lssl`

### Uso

`mpirun -np 4 bruteforce`

> Nota: Se debe especificar en el archivo `./input.txt`:
> 1. La llave privada para encriptar
> 2. El mensaje para encriptar
> 3. La *llave publica* (mensaje a buscar para verificar desincriptacion)
>
> Ejemplo:
>> 123456
>>
>> Esta es una prueba de proyecto 2
>>
>> es una prueba de
>>


## Autores
👤 Diego Cordova
- <a href= "https://github.com/Diego-CB">Github</a>

👤 Gabriela Contreras
- <a href="https://github.com/Paola-Contreras">Github</a>

👤 Andrés de la Roca  
- <a href = "https://www.linkedin.com/in/andr%C3%A8s-de-la-roca-pineda-10a40319b/">Linkedin</a>  
- <a href="https://github.com/andresdlRoca">Github</a>
