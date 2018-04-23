#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "aes.h"

#define DEBUG  

// Convert a string into a vector of bytes
vector<uint8_t> string_to_byte_vector(string input) {
  vector<uint8_t> byte_vector_;
  for (int i = 0; i < input.size(); i++) {
    uint8_t char_byte_ = (uint8_t) input[i]; 
    byte_vector_.emplace_back(char_byte_);
  }
  return byte_vector_;
}

// Generate a string of hex values from a byte vector
string hex_string(const vector<uint8_t> &bytes) {
  stringstream hex_bytes_;
  for(int i = 0; i<bytes.size(); i++){
    hex_bytes_ << hex << (int)bytes[i];
  }
  return hex_bytes_.str();
}

using namespace std;
int main(int argc, char *argv[]) {
  
  string key_file_name;
  string input_file_name;
  string output_file_name;
  string mode;

  for (int i = 1; i < argc; i++) {
    string param = string(argv[i]);
       
    if (param == "--keyfile") {
      key_file_name = string(argv[i + 1]);
    } else if (param == "--inputfile") {
      input_file_name = string(argv[i + 1]);
    } else if (param == "--outputfile") {
      output_file_name = string(argv[i + 1]);
    } else if (param == "--mode") {
      mode = string(argv[i + 1]);
    }
  }

#ifdef DEBUG
  cout << "DEBUG ==== " << "Key File Name : " << key_file_name << endl;
  cout << "DEBUG ==== " << "Input File Name : " << input_file_name << endl;
  cout << "DEBUG ==== " << "Output File Name : " << output_file_name << endl;
  cout << "DEBUG ==== " << "Mode : " << mode << endl;
#endif

  vector<uint8_t> key;
  vector<uint8_t> input;
  vector<uint8_t> output;
  string data;

  ifstream key_file(key_file_name);
  if(key_file.is_open()){
    while(getline(key_file, data)){
      key = string_to_byte_vector(data);
    }
  }
  key_file.close();
  ifstream input_file(input_file_name);
  if(input_file.is_open()){
    while(getline(input_file, data)){
      input = string_to_byte_vector(data);
    }
  }
  input_file.close();

#ifdef DEBUG
  cout << "DEBUG ==== " << "Key Data : " << key.data() << endl;
  cout << "DEBUG ==== " << "Input Data : " << input.data() << endl;
  cout << "DEBUG ==== " << "Key Hex Data : " << hex_string(key) << endl;
  cout << "DEBUG ==== " << "Input Hex Data : " << hex_string(input) << endl;
#endif

  //    ifstream key_file("key");
  //    string line;
  //    vector<uint8_t> bytes;
  //    if(key_file.is_open()) {
  //        while(getline(key_file, line)) {
  //            cout << "Our super secret key is " << line << endl;
  //            bytes = byte_vector(line);
  //        }
  //    }
  //    AES machine(bytes);
  //    cout << "Input text to encrypt: " << endl;
  //    string textfile;
  //    cin >> textfile;
  //    vector<uint8_t> text_ = byte_vector(textfile);
  //    bytes_to_output(machine.encrypt_this(text_), cout);
}
