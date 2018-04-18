#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdint.h>

#include "aes.h"
using namespace std;


uint8_t EasyWord::get_byte(int index) {
    assert(index >= 0 && index <= 3);
    char *byte_ptr = (char *) &word_;
    return  byte_ptr[index];
}

KeyMaster::KeyMaster(const vector<uint_8t>& _key) {
    key_Nb = (size_t) 4;
    key_Nk = (size_t) _key.size()/4;
    key_Nr = (size_t) (_key.size() == 16 ? 10:14);
    key_schedule = (uint32_t*) new int[key_Nb * (key_Nr+1)];
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
      input_to_state(block);

      for (uint8_t j = 0; j < master_.get_num_rounds() - 1; j++) {
        sub_bytes();
        shift_rows();
        mix_columns();
        add_round_key(master_.get_round_key());
      }

      sub_bytes();
      shift_rows();
      add_round_key(master_.get_round_key());

      vector<uint8_t> block_output = state_to_output();
      for (uint8_t j = 0; j < block_output.size(); j++) {
        _outputtext.push_back(block_output[j]);
      }
    }
}

void AES::input_to_state(const vector<uint_8t>& _input) {
  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      state_[r][c] = _input[r + 4 * c];
    }
  }
}

void AES::add_round_key(uint32_t _word) {
}

vector<uint8_t> AES::matrix_multiply(uint_8t matrix[4][4]) {
   for(int i = 0; i < 4; i++) {
        uint8_t new_col[4];
        //for every row value, we have to compute the matrix mult
        for(int j = 0; j < 4; j++) {
            //go through and multiply each val in column by matrix
            for(int k = 0; k < 4; k++) {
                new_col[j] += state_[k][i] * matrix[j][k];
            }
        }
        //we now set our state value to the new column
        for(int j = 0; j < 4; j++) {
            state_[j][i] = new_col[j];
        }
    }
}

void AES::shift_rows() {
}

void AES::mix_columns() {
    //given in whitepaper
    uint8_t mult_by_mat[4][4] = { {2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2,}};
    //for every column of state_ 
    matrix_multiply(mult_by_mat);
}

void AES::inv_mix_columns() {
    uint8_t mult_by_mat[4][4] = { { 0xe, 0xb, 0xd, 0x9}, { 0x9, 0xe, 0xb, 0xd}, {0xd, 0x9, 0xe, 0xb}, {0xb, 0xd, 0x9, 0xe}};
    matrix_multiply(mult_by_mat);    
}

void AES::sub_bytes() {
    //use the value within the state to index into S-box
    //then, assign current cell in state to value in S-box
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int index = static_cast<int>(state_[i][j]);
            state_[i][j] = S_TABLE[index];
        }
    }
}

vector<uint8_t> AES::state_to_output() {
  vector<uint8_t> output(20);

  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      output[r + 4 * c] = state_[r][c];
    }
  }

  return output;
}

uint32_t AES::sub_word(uint32_t _word) {
    
}
