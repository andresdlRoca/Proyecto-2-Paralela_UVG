#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Función para cifrar el texto
void encrypt(char *text, char *key) {
    int textLen = strlen(text);
    int keyLen = strlen(key);

    for (int i = 0; i < textLen; i++) {
        text[i] = text[i] ^ key[i % keyLen];
    }
}

// Función para descifrar el texto
void decrypt(char *text, char *key) {
    encrypt(text, key); // El cifrado y el descifrado son iguales en este caso
}

int main() {
    char *text = "TextoPrueba123";
    char ciphertext[50]; // Tamaño suficiente para almacenar el texto cifrado
    char key[50];       // Tamaño suficiente para almacenar la clave

    // Caracteres alfanuméricos permitidos
    char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    // Inicializar el generador de números aleatorios con una semilla

    printf("Ingrese la key: ");
    scanf("%s", key);

    // Obtener la longitud de la clave
    int keyLen = strlen(key);

    key[keyLen] = '\0';

    // Copiar el texto original en la variable ciphertext
    strcpy(ciphertext, text);

    // Cifrar el texto con la clave generada
    encrypt(ciphertext, key);

    // Aquí puedes medir el tiempo y realizar la búsqueda de la clave
    clock_t start_time = clock();

    // Implementa la búsqueda de fuerza bruta aquí
    char foundKey[50]; // Almacenará la clave encontrada
    for (int i = 0; i < keyLen; i++) {
        foundKey[i] = 'A';
    }
    foundKey[keyLen] = '\0';

    while (strcmp(foundKey, key) != 0) {
        // Generar la siguiente clave candidata
        for (int i = keyLen - 1; i >= 0; i--) {
            int index = strchr(alphanum, foundKey[i]) - alphanum;
            if (index < strlen(alphanum) - 1) {

                foundKey[i] = alphanum[index + 1];
            } else {
                foundKey[i] = alphanum[0];
            }
            if (foundKey[i] == 'A') {
                continue; // Carryover to the next character
            } else {
                break;
            }
        }
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Imprimir resultados
    printf("Key Length: %d, Found Key: %s, Elapsed Time: %f seconds\n", keyLen, foundKey, elapsed_time);


    return 0;
}
