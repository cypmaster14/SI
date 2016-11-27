#include <iostream>
#include <fstream>
#include <string>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstring>
#include <thread>
#include <fcntl.h>
#include <zconf.h>

using namespace std;

string plainText, cipherText;
string pt, ct, mode;


void initialize() {
    cout << "PlainText File:";
    cin >> pt;
    cout << "CipherTextFile:";
    cin >> ct;
    cout << "Mode:";
    cin >> mode;
}

string read_content(const char *filename) {
    ifstream plainTextFile(filename);
    string content = "";
    string line;

    if (plainTextFile.is_open()) {
        while (getline(plainTextFile, line)) {
            content += line;
        }
    }

    return content;
}


void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, const unsigned char *key,
            unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;

    cout << "cheie" << key << endl;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        return -1;

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        return -1;
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) return -1;
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


bool compare(unsigned char cipherText[128], unsigned char decryptedText[128], int decryptedtext_len) {

    for (int i = 0; i < decryptedtext_len; i++) {
        if (cipherText[i] != decryptedText[i]) {
            return false;
        }
    }
    return true;
}

char *padare_cuvant(char buffer[]) {
    int i = strlen(buffer);
    char *cheie = (char *) malloc(17);
    strcpy(cheie, buffer);
    for (; i < 16; i++) {
        strcat(cheie, "\x20");
    }
    cheie[i] = '\0';
    return cheie;
}


int main() {

    initialize();

    plainText = read_content(pt.c_str());
    cipherText = read_content(ct.c_str());


    /* Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */

    /* A 256 bit key */
    unsigned char *key = (unsigned char *) "median";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *) "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

    /* Message to be encrypted */
    unsigned char *plaintext = (unsigned char *) plainText.c_str();

    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
    unsigned char ciphertext[128];

//    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];
    int decryptedtext_len, ciphertext_len;

    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    /* Encrypt the plaintext */
    ciphertext_len = encrypt(plaintext, strlen((char *) plaintext), key, iv,
                             ciphertext);

    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp(stdout, (const char *) ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    string line;

    int d_fd = open("wordDict.txt", O_RDONLY);
    if (d_fd == -1) {
        perror("Opent pt file");
        exit(1);
    }
    char buffer[2], word[17];
    strcpy(word, "");
    int r;
    while ((r = read(d_fd, &buffer, 1)) > 0) {
        if (buffer[0] != '\n') {
            strcat(word, buffer);
        } else {

            char *aux = (char *) "median";

            cout << word << " " << strlen(word) << endl << aux << strlen(aux) << " " << endl;

//
//            chrono::milliseconds timespan(5000);
//            this_thread::sleep_for(timespan);
            char *cheie;

            if (strlen(word) < 16) {
                cheie = padare_cuvant(word);
            }

            if (strcmp(aux, cheie) == 0) {
                cout << "Sunt egale:(" << aux << "," << cheie << ")";
            } else {
                cout << "Nu sunt egale";
            }


            cout << "Cheie dupa padare" << cheie << endl;
            decryptedtext_len = decrypt(ciphertext, ciphertext_len, (const unsigned char *) cheie, iv,
                                        decryptedtext);
            decryptedtext[decryptedtext_len] = '\0';
            cout << decryptedtext << endl;
            strcpy(word, "");
        }
    }

//    ifstream dictionary("wordDict.txt");
//    char aux[4096];
//
//    while (dictionary >> aux) {
//
//        /* Buffer for the decrypted text */
//        cout << aux << " " << strlen(aux) << endl;
//        char *pointer = aux;
//        decryptedtext_len = decrypt(ciphertext, ciphertext_len, (const unsigned char *) "median",
//                                    iv,
//                                    decryptedtext);
//
//        cout << decryptedtext_len << endl;
//        /* Add a NULL terminator. We are expecting printable text */
//        decryptedtext[decryptedtext_len] = '\0';
//        cout << decryptedtext << endl;
//
//        chrono::milliseconds timespan(1000);
//        this_thread::sleep_for(timespan);
//        if (decryptedtext_len != -1) {
//
//            printf("%s\n", line.c_str());
//            printf("%s\n", decryptedtext);
//            printf("%s\n\n", plaintext);
//            chrono::milliseconds timespan(5000);
//            this_thread::sleep_for(timespan);
//            bool match = compare(plaintext, decryptedtext, decryptedtext_len);
//            if (match) {
//                /* Show the decrypted text */
//                printf("Decrypted text is:\n");
//                printf("%s\n", decryptedtext);
//                break;
//            }
//
//        }
//
//
//    }

    /* Clean up */
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}