//bruteforce.c
//nota: el key usado es bastante pequenio, cuando sea random speedup variara
// compilar: mpicc -o bruteforce bruteforce.c -lcrypto -lssl
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
        key <<= 1;
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

int tryKey(long key, char *ciph, int len, char search[]){
    char temp[len+1];
    memcpy(temp, ciph, len);
    temp[len]=0;
    decrypt(key, temp, len);
    return strstr((char *)temp, search) != NULL;
}

int main(int argc, char *argv[]){ //char **argv
    // MPi Init
    int N, id;
    long upper = (1L <<56); //upper bound DES keys 2^56
    long mylower, myupper;
    MPI_Status st;
    MPI_Request req;
    int flag;
    char keyLine[64];
    unsigned char cipherLine[64];
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
        printf("Mensaje Original: %s", cipherLine);
    }

    // Cifrar texto
    char buffer[512];
    encrypt(key, cipherLine, sizeof(cipherLine));
    fclose(fileIn);

    if (id == 0){
        printf("Mensaje Cifrado: %s\n", cipherLine);
        printf("\n------ Desencriptando por Bruteforce ------\n");
    }

    MPI_Barrier(MPI_COMM_WORLD); // Barrera MPI
    int ciphlen = strlen(cipherLine);
    if(id==0){
        tstart = MPI_Wtime();
    }

    // Descifrado
    int range_per_node = upper / N;
    mylower = range_per_node * id;
    myupper = range_per_node * (id+1) -1;
    if(id == N-1){
        //compensar residuo
        myupper = upper;
    }

    long found = 0;

    MPI_Irecv(&found, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &req);

    for(int i = mylower; i<myupper && (found==0); ++i){
        if(tryKey(i, (char *)cipherLine, ciphlen, search)){
        found = i;
        for(int node=0; node<N; node++){
            MPI_Send(&found, 1, MPI_LONG, node, 0, MPI_COMM_WORLD);
        }
        break;
        }
    }

    if(id==0){
        tend = MPI_Wtime();
        MPI_Wait(&req, &st);
        decrypt(found, (char *)cipherLine, ciphlen);
        printf("%li %s\n", found, cipherLine);
        printf("Took %f ms to run\n", (tend-tstart) * 1000);
    }

    MPI_Finalize();
}
