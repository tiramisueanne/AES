#include <vector>
#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "aes.h"

using namespace std;

TEST(StringToByteVector, CharCompare) {
  string sample = string("Thats my Kung Fu");
  vector<uint8_t> from_string = string_to_byte_vector(sample);
  for(int i = 0; i<from_string.size(); i++){
    ASSERT_EQ(from_string[i], sample[i]);
  }
}

TEST(StringToByteVector, CStrCompare) {
  char const* sample = "Thats my Kung Fu";
  vector<uint8_t> from_string = string_to_byte_vector(string(sample));
  ASSERT_STREQ((char const*)from_string.data(), sample);
}

