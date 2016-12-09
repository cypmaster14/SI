//
// Created by lazar on 27.11.2016.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commonFunctions.h"

int main(int argc, char *argv[]) {

    char *option = argv[1];
    char *plainText = read_content(argv[2]);
    char *cipherText = read_content(argv[3]);

    if (strcmp(option, "ECB") != 0 && strcmp(option, "CBC") != 0) {
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
    int i = 0;

    // I read an entry from dictionary until i found the encryption key
    while ((read = getline(&possibleKey, &possibleKeyLength, dictionary)) != -1) {
        removeNewLine(possibleKey);
        possibleKey = word_padding(possibleKey);
        unsigned char *possibleCipherText;
        int plainTextLength = strlen(plainText);

        if (strcmp(option, "ECB") == 0) {
            possibleCipherText = aes_encrypt_ecb((unsigned char *) plainText, &plainTextLength,
                                                 (unsigned char *) possibleKey);
        } else if (strcmp(option, "CBC") == 0) {
            possibleCipherText = aes_encrypt_cbc((unsigned char *) plainText, &plainTextLength,
                                                 (unsigned char *) possibleKey);
        }

        if (strcmp((const char *) possibleCipherText, cipherText) == 0) {
            printf("Cheia a fost gasita:%s\n", possibleKey);
            exit(0);
        }
    }

    fclose(dictionary);
    free(option);
    return 0;
}
