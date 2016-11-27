//
// Created by lazar on 27.11.2016.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>


unsigned char iv[] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

unsigned char *aes_encrypt_cbc(unsigned char *plaintext, int *len, unsigned char *key_data) {

    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(e, EVP_aes_128_cbc(), NULL, key_data, iv);
    int c_len = *len + 128;
    int f_len = 0;
    unsigned char *ciphertext = (unsigned char *) malloc(c_len);

    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
    EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

unsigned char *aes_encrypt_ecb(unsigned char *plaintext, int *len, unsigned char *key_data) {

    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(e, EVP_aes_128_ecb(), NULL, key_data, NULL);
    int c_len = *len + 128;
    int f_len = 0;
    unsigned char *ciphertext = (unsigned char *) malloc(c_len);

    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
    EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

unsigned char *aes_decrypt_cbc(unsigned char *ciphertext, int *len, unsigned char *key_data) {
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(e, EVP_aes_128_cbc(), NULL, key_data, iv);
    int p_len = *len, f_len = 0;
    unsigned char *plaintext = (unsigned char *) malloc(p_len);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

    *len = p_len + f_len;
    return plaintext;
}

unsigned char *aes_decrypt_ecb(unsigned char *ciphertext, int *len, unsigned char *key_data) {
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    EVP_CIPHER_CTX *e = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(e, EVP_aes_128_ecb(), NULL, key_data, iv);
    int p_len = *len, f_len = 0;
    unsigned char *plaintext = (unsigned char *) malloc(p_len);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

    *len = p_len + f_len;
    return plaintext;
}

char *padara_cuvant(char buffer[]) {
    int i = strlen(buffer);
    char *cheie = (char *) malloc(17);
    strcpy(cheie, buffer);
    for (; i < 16; i++) {
        strcat(cheie, "\x20");
    }
    cheie[i] = '\0';
    return cheie;
}

char *read_content(const char *filename) {

    FILE *f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_content = (char *) malloc(fsize + 1);
    fread(file_content, fsize, 1, f);
    fclose(f);
    return file_content;

}


int main() {

    char *plainText = read_content("pt.txt");
    char *optiune = (char *) malloc(10);
    printf("Mod:");
    scanf("%s", optiune);

    if (strcmp(optiune, "ECB") != 0 && strcmp(optiune, "CBC") != 0) {
        perror("Modul introdus nu este unul valid");
        exit(-1);
    }

    printf("%s\n", plainText);

    unsigned char *key_data;

    /* the key_data is read from the argument list */
    key_data = (unsigned char *) padara_cuvant((char *) "median");


    /* encrypt and decrypt each input string and compare with the original */

    ///Encrypt and decrypt

    unsigned char *cipherText;
    char *decrypredText;
    int olen, len;
    olen = len = strlen(plainText);

    if (strcmp(optiune, "ECB") == 0) {
        cipherText = aes_encrypt_ecb((unsigned char *) plainText, &len, key_data);
    } else if (strcmp(optiune, "CBC") == 0) {
        cipherText = aes_encrypt_cbc((unsigned char *) plainText, &len, key_data);
    }

    //Initialise decryption
    //Try every word in the dictionary

    FILE *dictionary = fopen("wordDict.txt", "r");
    if (dictionary == NULL) {
        exit(EXIT_FAILURE);
    }

    char *word = NULL;
    size_t wordLen = 0;
    ssize_t read;

    while ((read = getline(&word, &wordLen, dictionary)) != -1) {
        if (word[strlen(word) - 1] == '\n') {
            word[strlen(word) - 1] = '\0';
        }
        word = padara_cuvant(word);
        int aux = len;

        if (strcmp(optiune, "ECB") == 0) {
            decrypredText = (char *) aes_decrypt_ecb(cipherText, &aux, (unsigned char *) word);
        } else if (strcmp(optiune, "CBC") == 0) {
            decrypredText = (char *) aes_decrypt_cbc(cipherText, &aux, (unsigned char *) word);
        }

        if (strncmp(decrypredText, plainText, olen)) {
//            printf("FAIL: enc/dec failed for \"%s\"\n", plainText);
        } else {
            printf("\nOK: enc/dec ok for \"%s\"\n", decrypredText);
            exit(-1);
        }

        free(decrypredText);
    }

    free(cipherText);

    return 0;
}
