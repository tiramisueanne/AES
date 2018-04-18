#include <vector>
#include <cmath>
#include <iostream>
#include <stdint.h>

#include "aes.h"
using namespace std;

KeyMaster::KeyMaster(const vector<uint_8t>& _key) : key_size_Nk_(_key.size()/4) {

}


uint32_t KeyMaster::get_round_key() {
    return 42;
}

uint32_t KeyMaster::rotate_word(uint32_t _word) {
    return 42;
}

AES::AES(const vector<uint_8t>& _key): master_(_key){
}

void AES::encrypt_this(string _plaintext) {
    vector<uint8_t> _vectortext(_plaintext.begin(), _plaintext.end());
    vector<uint8_t> _outputtext;

    //if _plaintext size is not evenly divisible by 16
    //                                  (128b block size / 8b ints)
    //pad end with 0s
    uint8_t to_pad = 16 - (_vectortext.size() % 16);
    for (uint8_t i = 0; i < to_pad; i++) {
      _vectortext.push_back(0);
    }

    //for each 128b block (16 * 8b)
    for (uint32_t i = 0; i < _vectortext.size(); i += 16) {
      //get (16 * 8b) block
      vector<uint8_t> block;
      for (uint8_t j = 0; j < 16; j++) {
        block.push_back(_vectortext[i + j]);
      }
      //put block in state
      input_to_state(block)

      for (uint8_t j = 0; j < master_.get_num_rounds() - 1; j++) {
        sub_bytes()
        shift_rows()
        mix_columns()
        add_round_key(master_.get_round_key())
      }

      sub_bytes()
      shift_rows()
      add_round_key(master_.get_round_key())

      vector<uint8_t>& block_output = state_to_output()
      for (uint8_t j = 0; j < block_output.size(); j++) {
        _outputtext.push_back(block_output[j]);
      }
    }
}

void AES::input_to_state(const vector<uint_8t>& _input) {
  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      state_[r, c] = _input[r + 4 * c];
    }
  }
}

void AES::expand_key(const vector<uint_8t>& _key) {
}

void AES::add_round_key(uint32_t _word) {
}

void AES::shift_rows() {

}

void AES::mix_columns() {
}

void AES::inv_mix_columns() {
}

void AES::sub_bytes() {
}

vector<uint8_t>& AES::state_to_output() {
  vector<uint8_t> output(20);

  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      output[r + 4 * c] = state_[r, c];
    }
  }

  return output&;
}

uint32_t AES::sub_word(uint32_t _word) {

    return 42;
}
