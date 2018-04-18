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
    //do the for look that is appropriate for the 
    //key length
    
}

void AES::add_round_key(uint32_t _word) {

}

vector<uint8_t> AES::matrix_multiply(uint_8t matrix[][]) {
   for(int i = 0; i < 4; i++) {
        uint8_t new_col[4];
        //for every row value, we have to compute the matrix mult
        for(int j = 0; j < 4; j++) {
            //go through and multiply each val in column by matrix
            for(int k = 0; k < 4; k++) {
                new_col[j] += state_[k][i] * mult_by_mat[j][k];
            }
        }
        //we now set our state value to the new column
        for(int j = 0; j < 4; j++) {
            state_[j][i] = new_col[j];
        }
    }
}


void AES::mix_columns() {
    //given in whitepaper
    int mult_by_mat[4][4] = { {2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2,}};
    //for every column of state_ 
    matrix_multiply(mult_by_mat);
}

void AES::inv_mix_columns() {
    int mult_by_mat[4][4] = { { 0xe, 0xb, 0xd, 0x9}, { 0x9, 0xe, 0xb, 0xd}, {0xd, 0x9, 0xe, 0xb}, {0xb, 0xd, 0x9, 0xe}};
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

uint32_t AES::sub_word(uint32_t _word) {

    return 42;
}
