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

void AES::mix_columns() {
}

void AES::inv_mix_columns() {
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
