//
// Created by lazar on 28.11.2016.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include "commonFunctions.h"


int main(int argc, char *argv[]) {

    char *optiune = argv[1];
    char *plainText = read_content(argv[3]);
    int plainTextLen = strlen(plainText);

    if (strcmp(optiune, "ECB") != 0 && strcmp(optiune, "CBC") != 0) {
        perror("Modul introdus nu este unul valid");
        exit(-1);
    }

    unsigned char *key_data;
    int key_number = atoi(argv[2]);
    printf("Indice Cheie:%d\n", key_number);

    FILE *dictionary = fopen("wordDict.txt", "r");

    char *word = NULL;
    size_t wordLen = 0;

    //I read the N-th key from dictionary (N is the random number
    for (int i = 0; i < key_number; i++) {
        getline(&word, &wordLen, dictionary);
    }
    fclose(dictionary);

    //getline() -> returns an entire line and it reads even the \n character if exists
    removeNewLine(word);

    key_data = (unsigned char *) word_padding(word);
    printf("Cheie:%s", key_data);
    unsigned char *cipherText;

    if (strcmp(optiune, "ECB") == 0) {
        cipherText = aes_encrypt_ecb((unsigned char *) plainText, &plainTextLen, key_data);
    } else if (strcmp(optiune, "CBC") == 0) {
        cipherText = aes_encrypt_cbc((unsigned char *) plainText, &plainTextLen, key_data);
    }

    //I write the cipherText into cipherTextFile
    FILE *ciphetTextFile = fopen(argv[4], "w");
    fprintf(ciphetTextFile, "%s", (const char *) cipherText);
    fclose(ciphetTextFile);
    printf("\n");
    return 0;
}
