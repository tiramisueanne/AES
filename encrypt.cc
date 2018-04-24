#include <fstream>
#include <iostream>
#include <string>

#include "aes.h"

#define DEBUG

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
  cout << "DEBUG ==== "
       << "Key File Name : " << key_file_name << endl;
  cout << "DEBUG ==== "
       << "Input File Name : " << input_file_name << endl;
  cout << "DEBUG ==== "
       << "Output File Name : " << output_file_name << endl;
  cout << "DEBUG ==== "
       << "Mode : " << mode << endl;
#endif

  vector<uint8_t> key;
  vector<uint8_t> input;
  vector<uint8_t> output;
  string data;

  ifstream key_file(key_file_name);
  if (key_file.is_open()) {
    while (getline(key_file, data)) {
      key = string_to_byte_vector(data);
    }
  }
  key_file.close();
  ifstream input_file(input_file_name);
  if (input_file.is_open()) {
    while (getline(input_file, data)) {
      input = string_to_byte_vector(data);
    }
  }
  input_file.close();

#ifdef DEBUG
  cout << "DEBUG ==== "
       << "Key Data : " << key.data() << endl;
  cout << "DEBUG ==== "
       << "Input Data : " << input.data() << endl;
  cout << "DEBUG ==== "
       << "Key Hex Data : " << hex_string(key) << endl;
  cout << "DEBUG ==== "
       << "Input Hex Data : " << hex_string(input) << endl;
#endif

  AES aes_machine(key);
  if (mode == "encrypt") {
    output = aes_machine.encrypt_this(input);
  } else if (mode == "decrypt") {
    output = aes_machine.decrypt_this(input);
  } else {
    cout << "Invalid Mode, Please type \"encrypt\" or \"decrypt\"" << endl;
    exit(1);
  }

#ifdef DEBUG
  cout << "DEBUG ==== "
       << "Output Data : " << output.data() << endl;
  cout << "DEBUG ==== "
       << "Output Hex Data : " << hex_string(output) << endl;
#endif

  ofstream output_file(output_file_name);
  if (output_file.is_open()) {
	for(int i = 0; i < output.size(); i++) {
		output_file << output[i];
	}
  }
  output_file.close();
}
