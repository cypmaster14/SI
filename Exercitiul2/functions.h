//
// Created by lazar on 27.11.2016.
//

#ifndef EXERCITIUL2_FUNCTIONS_H_H
#define EXERCITIUL2_FUNCTIONS_H_H

#endif //EXERCITIUL2_FUNCTIONS_H_H

using namespace std;

string read_content(char *filename) {
    /*
     * Function that returns the content of the file
     */
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
    /*
     * Function that writes the SHA256 value of the content into the output file
     */
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
    /*
     * Function that writes the MD5 value of the content into the output file
     */
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


int getNumberOfSimilarBytes(string h1, string h2) {
    /*
     * Function that return the number of equal bytes
     *
     */
    int identicalBytes = 0;
    for (int i = 0; i < h1.length(); i = i + 2) {
        if (h1[i] == h2[i] && h1[i + 1] == h2[i + 1]) {
            identicalBytes++;
        }
    }
    return identicalBytes;
}