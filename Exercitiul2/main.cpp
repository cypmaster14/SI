#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <fstream>
#include <iostream>

using namespace std;

string f1_content;
string f2_content;
string h1_sha256Content, h2_sha256Content, h1_md5, h2_md5;

string read_content(char *filename) {
    ifstream file(filename);
    string content = "";
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            content += line;
        }
        file.close();
    }

    return content;
}


void getSha256(const char *content, string outputFile) {
    SHA256_CTX ctx;
    unsigned char result[SHA256_DIGEST_LENGTH];
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, content, strlen(content));
    SHA256_Final(result, &ctx);
    string sha256 = "";
    FILE *sha256File = fopen(outputFile.c_str(), "w");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        fprintf(sha256File, "%02x", result[i]);
    }
    fclose(sha256File);
}


void getMD5(const char *content, string outputFile) {
    MD5_CTX ctx;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Init(&ctx);
    MD5_Update(&ctx, content, strlen(content));
    MD5_Final(result, &ctx);
    string md5 = "";
    FILE *md5File = fopen(outputFile.c_str(), "w");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        fprintf(md5File, "%02x", result[i]);
    }
    fclose(md5File);
}


int compareHash(string h1, string h2) {
    int identicalBytes = 0;
    for (int i = 0; i < h1.length(); i++) {
        if (h1[i] == h2[i]) {
            identicalBytes++;
        }
    }
    return identicalBytes;
}

int main() {

    f1_content = read_content((char *) "f1.txt");
    f2_content = read_content((char *) "f2.txt");
    getSha256(f1_content.c_str(), "h1_sha256.txt");
    getSha256(f2_content.c_str(), "h2_sha256.txt");
    getMD5(f1_content.c_str(), "h1_md5.txt");
    getMD5(f2_content.c_str(), "h2_md5.txt");

    h1_sha256Content = read_content((char *) "h1_sha256.txt");
    h2_sha256Content = read_content((char *) "h2_sha256.txt");
    h1_md5 = read_content((char *) "h1_md5.txt");
    h2_md5 = read_content((char *) "h2_md5.txt");

    cout << "H1SHA256:" << h1_sha256Content << "\n";
    cout << "H2SHA256:" << h2_sha256Content << "\n";
    cout << "Identical bytes for SHA256:" << compareHash(h1_sha256Content, h2_sha256Content) << "\n";

    cout << "H1MD5:" << h1_md5 << "\n";
    cout << "H2MD5:" << h2_md5 << "\n";
    cout << "Identical bytes for MD5:" << compareHash(h1_md5, h2_md5) << "\n";


    return 0;
}