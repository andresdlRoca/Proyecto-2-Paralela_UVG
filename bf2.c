//bruteforce.c
//nota: el key usado es bastante pequenio, cuando sea random speedup variara
// compilar: mpicc -o bruteforce bruteforce.c -lcrypto
// ejecutar: mpirun -np 4 bruteforce

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <openssl/des.h>

void decrypt(long key, char *ciph, int len) {
    DES_key_schedule keysched;
    DES_cblock des_key;

    // Set parity of key
    for (int i = 0; i < 8; ++i) {
        des_key[i] = (key & (0xFE << i * 8)) >> (i * 8);
    }

    // Initialize the key schedule
    DES_set_odd_parity(&des_key);
    DES_set_key_checked(&des_key, &keysched);

    // Decrypt the data
    for (int i = 0; i < len; i += 8) {
        DES_ecb_encrypt((DES_cblock *)(ciph + i), (DES_cblock *)(ciph + i), &keysched, DES_DECRYPT);
    }
}

void encrypt(long key, char *ciph, int len) {
    DES_key_schedule keysched;
    DES_cblock des_key;

    // Set parity of key
    for (int i = 0; i < 8; ++i) {
        key <<= 1;
        des_key[i] = (key & (0xFE << i * 8)) >> (i * 8);
    }

    // Initialize the key schedule
    DES_set_odd_parity(&des_key);
    DES_set_key_checked(&des_key, &keysched);

    // Encrypt the data
    for (int i = 0; i < len; i += 8) {
        DES_ecb_encrypt((DES_cblock *)(ciph + i), (DES_cblock *)(ciph + i), &keysched, DES_ENCRYPT);
    }
}

int tryKey(long key, char *ciph, int len,char search[]){
  char temp[len+1];
  memcpy(temp, ciph, len);
  temp[len]=0;
  decrypt(key, temp, len);
  return strstr((char *)temp, search) != NULL;
}


int main(int argc, char *argv[]){ //char **argv
  int flag;
  int N, id;
  long mylower, myupper;
  long upper = (1L <<56); //upper bound DES keys 2^56
 
  MPI_Status st;
  MPI_Request req;

 
  char keyLine[64];
  unsigned char cipherLine[64];
  int ciphlen = strlen(cipherLine);
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(comm, &N);
  MPI_Comm_rank(comm, &id);
  double tstart, tend;


  // Lectura de txt
  char inputFileName[] = "input.txt"; // Nombre del archivo de entrada
  char search[64]; // Cadena de búsqueda

  // Abrir el archivo de entrada
  FILE *fileIn = fopen(inputFileName, "r");
  if (fileIn == NULL) {
    perror("Error al abrir el archivo de entrada");
    MPI_Finalize();
    return 1;
  }
  
  // Leer la clave
  if (fgets(keyLine, sizeof(keyLine), fileIn) == NULL) {
    perror("Error al leer la clave del archivo");
    fclose(fileIn);
    return 1;
  }

  long key = atol(keyLine); // Convertir la clave a long
  if (id == 0){
    printf("------ Encriptacion ------\n");
    printf("Llave privada a utilizar: %ld\n", key);
  }

 // Leer el texto cifrado
  if (fgets(cipherLine, sizeof(cipherLine), fileIn) == NULL) {
    perror("Error al leer el texto cifrado del archivo");
    fclose(fileIn);
    return 1;
  }

  // Leer cadena de busqueda
  if (fgets(search, sizeof(search), fileIn) == NULL) {
    perror("Error al leer el texto cifrado del archivo");
    fclose(fileIn);
    return 1;
  }

  if (id == 0){
    printf("Mensaje Original: %s\n", cipherLine);
  }

  // Cifrar texto
  char buffer[512];
  encrypt(key, cipherLine, sizeof(cipherLine));
  fclose(fileIn);

 if (id == 0){
    printf("Mensaje Cifrado: %s\n", cipherLine);
    printf("------ Desencriptando por Bruteforce ------\n");
  }
 
  // Descifrado
    long found = 0;
    int range_per_node = upper / N;

    mylower = range_per_node * id;
    myupper = range_per_node * (id+1) -1;

    if(id == N-1){
        myupper = upper; //compensar residuo
    }

for (long currentKey = mylower; currentKey <= myupper; currentKey++) {

        if (tryKey(currentKey, cipherLine, sizeof(cipherLine), search)) {
            found = currentKey;
            printf("%ld",found);
            break; // Salir del bucle si se encuentra la clave
        }
    }

    // Comunicar el resultado
    if (found > 0) {
        // Clave encontrada en este nodo
        printf("Clave encontrada en el nodo %d: %ld\n", id, found);
    }

    // Realizar una reducción de máximo para encontrar la clave en todos los nodos
    MPI_Allreduce(&found, &found, 1, MPI_LONG, MPI_MAX, comm);

    if (id == 0) {
        if (found > 0) {
            printf("Clave encontrada: %ld\n", found);
        } else {
            printf("La clave no fue encontrada en el rango de claves.\n");
        }
    }

    MPI_Finalize();
    return 0;
}
