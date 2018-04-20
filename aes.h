#include <cmath>
#include <functional>
#include <string>
#include <stdint.h>
#include <vector>
using namespace std;

typedef unsigned char uint_8t;
typedef unsigned int uint_32t;

//this is the number of words in each round key
//allows for easy access into a uint32_t
class EasyWord {
public:
    uint32_t word_;
    EasyWord() {};
    EasyWord(uint32_t _word): word_(_word) {};
    uint8_t get_byte(int index);
    void set_byte(int index, uint8_t _val);
};

//could possibly use with EasyWord but it was too much work
class KeyMaster {
public:
    KeyMaster(const vector<uint_8t>& _key);

    int get_num_rounds() { return key_Nr; };

    uint32_t get_round_key();
    //number of rounds ( 128 bit - 10 | 256 bit - 14 )
    const size_t key_Nr;

    //I don't know if this is the easiest or not yet
    EasyWord *key_;
    //number of words in a state ( 128 bit - 4 | 256 bit - 4 )
    const size_t key_Nb;
    //number of words in a key ( 128 bit - 4 | 256 bit - 8 )
    const size_t key_Nk;
    //linear array of key_words, 4 for the start plus 4 for each round

private:

    //move leftmost byte to become the rightmost byte
    uint32_t rotate_word(uint32_t _word);

    //perform simple s-box substitution
    uint32_t sub_word(uint32_t _word);

    //magic function to calculate the first word per round
    uint32_t magic(uint32_t _word, int _round);

    //adds four new words to the key_schedule using the previous four
    void add_four_words(int _ks_idx);
};

class AES {
public:
    AES(const vector<uint_8t>& _key);

    // round function -
    //    for each block of set size
    //      copy into state (input_to_state)
    //      for each round -
    //        1. byte substitute using substitution table (sub_bytes)
    //        2. shift rows of state_ array by different outsets (shift_rows)
    //        3. mix data within each column of the state_ array (mix_columns)
    //        4. add round Key to State (from KeyMaster::get_round_key)
    void encrypt_this(string _plaintext);

    void decrypt_this(string _ciphertext);

private:

    //oooooh goodness me
    KeyMaster master_;
    const size_t Nb_;
    const size_t Nk_;
    const size_t Nr_;
    uint8_t state_[4][4];

    //moves ((block length) / 32) bytes of input into state array
    void input_to_state(const vector<uint8_t>& input);

    //Round key is added to the state
    void add_round_key(uint32_t _word);

    //shift state array rows by different offsets
    void shift_rows();

    //shift state array rows but backward
    void inv_shift_rows();

    void matrix_multiply(function<uint8_t(uint8_t)> matrix[4][4]);

    //mix column data within state
    void mix_columns();

    //The inverse of mix_columns
    void inv_mix_columns();

    
    //utilize S-box to change state bytes
    void sub_bytes();

    //inverse of sub_bytes
    void inv_sub_bytes();

    //moves ((block length) / 32) bytes of state array to output
    vector<uint8_t> state_to_output();
    
        
};
