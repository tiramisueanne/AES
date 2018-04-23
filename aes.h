#include <cmath>
#include <functional>
#include <stdint.h>
#include <string>
#include <vector>

#include "gtest/gtest.h"
using namespace std;

// Convert a string into a vector of bytes
vector<uint8_t> string_to_byte_vector(string input);

// Generate a string of hex values from a byte vector
string hex_string(const vector<uint8_t> &bytes);

// this is the number of words in each round key
// allows for easy access into a uint32_t
class EasyWord {

public:
  EasyWord(){};
  EasyWord(uint32_t _word) : word_(_word){};
  EasyWord(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3); 
  EasyWord &operator=(uint32_t _word) {
    word_ = _word;
    return *this;
  };
  operator uint32_t() const { return word_; };
  
  //This is big endian
  uint8_t get_byte(int index);
  void set_byte(int index, uint8_t _val);

private:
  uint32_t word_;
};

class KeyMaster {
public:
  KeyMaster(const vector<uint8_t> &_key);
  // return the total number of rounds, 10 for 128 bit key, 14 for 256 bit
  int get_num_rounds() { return key_Nr; };
  // get the next word in the key schedule
  uint32_t get_next_word();

private:
  // number of rounds ( 128 bit - 10 | 256 bit - 14 )
  const size_t key_Nr;
  // number of words in a state ( 128 bit - 4 | 256 bit - 4 )
  const size_t key_Nb;
  // number of words in a key ( 128 bit - 4 | 256 bit - 8 )
  const size_t key_Nk;
  // linear array of EasyWords, 4 for the start plus 4 for each round
  EasyWord *key_schedule_;
  // create a key schedule using a 128 bit key
  void generate_128_bit_key_schedule(const vector<uint8_t> &_key);
  // create a key schedule using a 256 bit key
  void generate_256_bit_key_schedule(const vector<uint8_t> &_key);
  // move leftmost byte to become the rightmost byte
  uint32_t rotate_word(EasyWord _word);

  // perform simple s-box substitution
  uint32_t sub_word(EasyWord _word);

  // adds four new words to the key_schedule using the previous four
  void add_four_words(int _ks_idx);
 
  void add_eight_words(int _ks_idx);

  // magic function to calculate the first word per round
  uint32_t magic(EasyWord _word, int _round);
};

class AES {
  friend struct AesTest;
  FRIEND_TEST(AesTest, InitState);
  FRIEND_TEST(AesTest, InitKey);  
public:
  AES(const vector<uint8_t> &_key) : master_(_key){};

  // round function -
  //    for each block of set size
  //      copy into state (input_to_state)
  //      for each round -
  //        1. byte substitute using substitution table (sub_bytes)
  //        2. shift rows of state_ array by different outsets (shift_rows)
  //        3. mix data within each column of the state_ array (mix_columns)
  //        4. add round Key to State (from KeyMaster::get_round_key)

  vector<uint8_t> encrypt_this(vector<uint8_t> &_vectortext);

  vector<uint8_t> decrypt_this(vector<uint8_t> &_vectortext);

private:
  // oooooh goodness me
  KeyMaster master_;
  uint8_t state_[4][4];

  // moves ((block length) / 32) bytes of input into state array
  void input_to_state(const vector<uint8_t> &_input);

  // Round key is added to the state
  void add_round_key();

  // shift state array rows by different offsets
  void shift_rows();

  // inverse of shift_rows
  void inv_shift_rows();

  // multiplies matricies
  void matrix_multiply(function<uint8_t(uint8_t)> matrix[4][4]);

  // mix column data within state
  void mix_columns();

  // The inverse of mix_columns
  void inv_mix_columns();

  // utilize S-box to change state bytes
  void sub_bytes();

  void inv_sub_bytes();

  // moves ((block length) / 32) bytes of state array to output
  vector<uint8_t> state_to_output();
};
