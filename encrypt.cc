#include <bitset>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "aes.h"

uint8_t ascii_to_byte(char c1, char c2) {
    //let's try out some behavior
    const char c_style[2] = {c1, c2};
    uint8_t byte_;
    byte_ = strtol(c_style, nullptr, 16);
    return byte_;
}

vector<uint8_t> byte_vector(string line) {
    vector<uint8_t> bytes;
    for(int i = 0; i <= line.size() - 2; i+=2) {
        uint8_t byte_ = ascii_to_byte(line[i], line[i+1]);
        bytes.emplace_back(byte_); 
    }
    return bytes;
}

void bytes_to_output(const vector<uint8_t> &bytes, ostream& oss) {
    for(int i = 0; i < bytes.size(); i++) {
        uint8_t byte = bytes[i];
        oss << hex << byte; 
    }
}

using namespace std;
int main() {
    ifstream key_file("aes_doc_256");
    stringstream ss;
    string line;
    vector<uint8_t> bytes;
    if(key_file.is_open()) {
        while(getline(key_file, line)) {
            cout << "Our super secret key is " << line << endl;
            bytes = byte_vector(line);
        }
    }
    AES machine(bytes);
    cout << "Input text to encrypt: " << endl;
    string textfile;
    cin >> textfile;
    vector<uint8_t> text_ = byte_vector(textfile);
    bytes_to_output(machine.encrypt_this(text_), cout);
}
