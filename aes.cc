#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

void KeyMaster::KeyMaster(const vector<uint_8t>& _key) {
}

uint32_t KeyMaster::get_round_key() {
}

uint32_t KeyMaster::rotate_word(uint32_t _word) {
    return 42;
}

AES::AES(const vector<uint_8>& _key): key_size_(_key.size()) {
    //Check if key_size is appropriate, then throw exception
    //if not 
    //if(key_size_ 
}

void AES::encrypt_this(string _plaintext) {
    //do the for look that is appropriate for the 
    //key length
    
    //cout the product of our encryption
}

void AES::expand_key(const vector<uint_8t>& _key) {
}

void AES::add_round_key(uint32_t _word) {
}

void AES::mix_columns() {
}

void AES::inv_mix_columns() {
}

void AES::sub_bytes() {
}

uint32_t AES::sub_word(uint32_t _word) {

    return 42;
}
