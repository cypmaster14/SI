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

    char *plainText = read_content("pt.txt");
    int plainTextLen = strlen(plainText);
//    char *optiune = (char *) malloc(10);
//    printf("Mod:");
//    scanf("%s", optiune);
    char *optiune = argv[1];

    if (strcmp(optiune, "ECB") != 0 && strcmp(optiune, "CBC") != 0) {
        perror("Modul introdus nu este unul valid");
        exit(-1);
    }

    unsigned char *key_data;
    int key_number = atoi(argv[2]);
    printf("Cheie:%d", key_number);

    FILE *dictionary = fopen("wordDict.txt", "r");

    char *word = NULL;
    size_t wordLen = 0;
    for (int i = 0; i < key_number; i++) {
        getline(&word, &wordLen, dictionary);
    }
    fclose(dictionary);

    removeNewLine(word);

    key_data = (unsigned char *) padara_cuvant(word);
    printf("Cheie%s:", key_data);
    unsigned char *cipherText;

    if (strcmp(optiune, "ECB") == 0) {
        cipherText = aes_encrypt_ecb((unsigned char *) plainText, &plainTextLen, key_data);
    } else if (strcmp(optiune, "CBC") == 0) {
        cipherText = aes_encrypt_cbc((unsigned char *) plainText, &plainTextLen, key_data);
    }

    FILE *ciphetTextFile = fopen("ct.txt", "w");
    fprintf(ciphetTextFile, "%s", (const char *) cipherText);
    fclose(ciphetTextFile);
    printf("\n");
    return 0;
}