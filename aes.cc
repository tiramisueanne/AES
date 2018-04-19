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

KeyMaster::KeyMaster(const vector<uint_8t>& _key):
    key_Nb(4),
    key_Nk(_key.size()/key_Nb),
    key_Nr(_key.size() == 16 ? 10:14) {
    key_schedule = new uint32_t[key_Nb * (key_Nr+1)];

    //key_schedule algorithm, use first four words to generate the next four
    //repeat until you have enough for every

    //transfer the first four words of the key to the schedule
    for(int i = 0; i < key_Nb; i++){
        uint_8t new_word[4];
        uint32_t word_to_put;
        for(int j = 0; j < 4; j++) {
            new_word[j] = _key[i*4 + j];
        }
        //this is a very dangerous thing to do, but
        //it would be how do.
        uint32_t *scary_ptr = reinterpret_cast<uint32_t*>(new_word);
	    key_schedule[i] =  *scary_ptr;
    }

    //add remaining blocks of four words to key schedule
    for(int i = 0; i < key_Nr; i++) {
	    add_four_words((i+1)*4);
    }

}
//adds four new words to the key_schedule using the previous four
void KeyMaster::add_four_words(int _ks_idx){
    //run magic function to generate magic word
    uint32_t magic_word = magic(key_schedule[_ks_idx-1], (int)_ks_idx/key_Nb);
    //add first value
    key_schedule[_ks_idx] = key_schedule[_ks_idx-4] ^ magic_word;
    //add second value
    key_schedule[_ks_idx+1] = key_schedule[_ks_idx] ^ key_schedule[_ks_idx-3];
    //add third value
    key_schedule[_ks_idx+2] = key_schedule[_ks_idx+1] ^ key_schedule[_ks_idx-2];
    //add fourth value
    key_schedule[_ks_idx+3] = key_schedule[_ks_idx+2] ^ key_schedule[_ks_idx-1];

int KeyMaster::get_num_rounds() {
  switch(key_size_Nk_) {
    case 4: return 10;
    case 8: return 14;
    default: return 10;
  }
}

//magic function to calculate first word per round
uint32_t KeyMaster::magic(uint32_t _word, int _round) {
    uint32_t current_word = _word;

    current_word = rotate_word(current_word);
    current_word = sub_word(current_word);
    //xor leftmost byte with precomputed ROUND_CONSTANT value
    uint32_t round_const = (ROUND_CONSTANT[_round] << 24);
    current_word = current_word ^ round_const;

    return current_word;
}

//move leftmost byte to become the rightmost byte
uint32_t KeyMaster::rotate_word(uint32_t _word) {
    uint8_t rotated_word[4];
    uint8_t* byte_pointer = (uint8_t*) &_word;
    rotated_word[0] = byte_pointer[1];
    rotated_word[1] = byte_pointer[2];
    rotated_word[2] = byte_pointer[3];
    rotated_word[3] = byte_pointer[0];
    return *(reinterpret_cast<uint32_t *>(rotated_word));
}

//perform simple s-box substitution
uint32_t KeyMaster::sub_word(uint32_t _word) {
    uint8_t subbed_word[4];
    uint8_t* byte_pointer = (uint8_t*) &_word;
    for(int i = 0; i < 4; i++){
        subbed_word[i] = S_TABLE[byte_pointer[i]];
    }
    return *(reinterpret_cast<uint32_t *>(subbed_word));

}

AES::AES(const vector<uint_8t>& _key):
    master_(_key),
    Nb_(master_.key_Nb),
    Nk_(master_.key_Nk),
    Nr_(master_.key_Nr)
{
    key_schedule_ = master_.key_schedule;
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

      add_round_key(master_.get_round_key());

      for (int j = 0; j < master_.get_num_rounds() - 1; j++) {
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

void AES::decrypt_this(string _ciphertext) {
  vector<uint8_t> _vectortext(_ciphertext.begin(), _ciphertext.end());
  vector<uint8_t> _outputtext;

  //for each 128b block (16 * 8b)
  for (uint32_t i = 0; i < _vectortext.size(); i += 16) {
    //get (16 * 8b) block
    vector<uint8_t> block;
    for (uint8_t j = 0; j < 16; j++) {
      block.push_back(_vectortext[i + j]);
    }
    //put block in state
    input_to_state(block);

    add_round_key(master_.get_round_key());

    for (int j = 0; j < master_.get_num_rounds() - 1; j++) {
      inv_shift_rows();
      inv_sub_bytes();
      add_round_key(master_.get_round_key());
      inv_mix_columns();
    }

    inv_shift_rows();
    inv_sub_bytes();
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

void AES::expand_key(const vector<uint_8t>& _key) {

}

void AES::add_round_key(uint32_t _word) {

}

void AES::matrix_multiply(uint_8t matrix[4][4]) {
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

//bytes in the last three rows of state_ are shifted by different offsets
// s0,0   s0,1    s0,2    s0,3           s0,0   s0,1    s0,2    s0,3
// s1,0   s1,1    s1,2    s1,3    --->   s1,1   s1,2    s1,3    s1,0  (offset 1)
// s2,0   s2,1    s2,2    s2,3    --->   s2,2   s2,3    s2,0    s2,1  (offset 2)
// s3,0   s3,1    s3,2    s3,3           s3,3   s3,0    s3,1    s3,2  (offset 3)
void AES::shift_rows() {
  uint8_t new_state[4][4] = {
    {state_[0][0], state_[0][1], state_[0][2], state_[0][3]},
    {state_[1][1], state_[1][2], state_[1][3], state_[1][0]},
    {state_[2][2], state_[2][3], state_[2][0], state_[2][1]},
    {state_[3][3], state_[3][0], state_[3][1], state_[3][2]}
  };
  for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
          state_[i][j] = new_state[i][j];
      }
  }
}

void AES::inv_shift_rows() {

}

void AES::mix_columns() {
    //given in whitepaper
    uint8_t mult_by_mat[4][4] = {
      {2, 3, 1, 1},
      {1, 2, 3, 1},
      {1, 1, 2, 3},
      {3, 1, 1, 2}
    };
    //for every column of state_
    matrix_multiply(mult_by_mat);
}

void AES::inv_mix_columns() {
    uint8_t mult_by_mat[4][4] = {
      {0xe, 0xb, 0xd, 0x9},
      {0x9, 0xe, 0xb, 0xd},
      {0xd, 0x9, 0xe, 0xb},
      {0xb, 0xd, 0x9, 0xe}
    };
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

void AES::inv_sub_bytes() {

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
  return 42;
}
