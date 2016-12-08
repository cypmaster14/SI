//
// Created by lazar on 28.11.2016.
//


#include <openssl/conf.h>
#include <openssl/evp.h>

#ifndef EXERCITIUL1_COMMONFUNCTIONS_H
#define EXERCITIUL1_COMMONFUNCTIONS_H

#endif //EXERCITIUL1_COMMONFUNCTIONS_H


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
    char *cheie = (char *) malloc(50);
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

void removeNewLine(char *word) {
    if (word[strlen(word) - 1] == '\n') {
        word[strlen(word) - 1] = '\0';
    }
}