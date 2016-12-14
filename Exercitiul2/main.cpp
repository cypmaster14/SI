#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <fstream>
#include <iostream>
#include "functions.h"

using namespace std;

string f1_content;
string f2_content;
string h1_sha256Content, h2_sha256Content, h1_md5, h2_md5;


int main(int argc, char *argv[]) {

    f1_content = read_content((char *) argv[1]);
    f2_content = read_content((char *) argv[2]);
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
    cout << "Identical bytes for SHA256:" << getNumberOfSimilarBytes(h1_sha256Content, h2_sha256Content) << "\n";

    cout << "H1MD5:" << h1_md5 << "\n";
    cout << "H2MD5:" << h2_md5 << "\n";
    cout << "Identical bytes for MD5:" << getNumberOfSimilarBytes(h1_md5, h2_md5) << "\n";


    return 0;
}