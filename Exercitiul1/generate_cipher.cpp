//
// Created by lazar on 28.11.2016.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include "commonFunctions.h"


int main() {

    char *plainText = read_content("pt.txt");
    int plainTextLen = strlen(plainText);
    char *optiune = (char *) malloc(10);
    printf("Mod:");
    scanf("%s", optiune);

    if (strcmp(optiune, "ECB") != 0 && strcmp(optiune, "CBC") != 0) {
        perror("Modul introdus nu este unul valid");
        exit(-1);
    }

    unsigned char *key_data;
    int key_number = rand() % 500;

    FILE *dictionary = fopen("wordDict.txt", "r");

    char *word = NULL;
    size_t wordLen = 0;
    for (int i = 0; i < 4; i++) {
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
    return 0;
}