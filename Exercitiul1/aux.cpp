//
// Created by lazar on 27.11.2016.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commonFunctions.h"

int main() {

    char *plainText = read_content("pt.txt");
    char *cipherText = read_content("ct.txt");
    char *optiune = (char *) malloc(10);
    printf("Mod:");
    scanf("%s", optiune);

    if (strcmp(optiune, "ECB") != 0 && strcmp(optiune, "CBC") != 0) {
        perror("Modul introdus nu este unul valid");
        exit(-1);
    }

    printf("%s\n", plainText);

    FILE *dictionary = fopen("wordDict.txt", "r");
    if (dictionary == NULL) {
        perror("Failed to open the dictionary");
        exit(EXIT_FAILURE);
    }

    char *possibleKey = NULL;
    size_t possibleKeyLength = 0;
    size_t read;

    while ((read = getline(&possibleKey, &possibleKeyLength, dictionary)) != -1) {
        removeNewLine(possibleKey);
        printf("Key size:%d(%s)\n", (int) strlen(possibleKey), possibleKey);
        possibleKey = padara_cuvant(possibleKey);
        unsigned char *possibleCipherText;
        int plainTextLength = strlen(plainText);

        if (strcmp(optiune, "ECB") == 0) {
            possibleCipherText = aes_encrypt_ecb((unsigned char *) plainText, &plainTextLength,
                                                 (unsigned char *) possibleKey);
        } else if (strcmp(optiune, "CBC") == 0) {
            possibleCipherText = aes_encrypt_cbc((unsigned char *) plainText, &plainTextLength,
                                                 (unsigned char *) possibleKey);
        }

        if (strcmp((const char *) possibleCipherText, cipherText) == 0) {
            printf("Cheia a fost gasita:%s", possibleKey);
            exit(0);
        }

    }

    fclose(dictionary);
    free(optiune);
    return 0;
}
