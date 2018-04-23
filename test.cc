#include <vector>
#include <string>
#include <iostream>
#include <bitset>
#include "gtest/gtest.h"
#include "aes.h"

using namespace std;

TEST(StringToByteVector, CharCompare) {
  string sample = "Thats my Kung Fu";
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

TEST(HexString, CharHex){
  vector<uint8_t> kung_fu_vector = {'T','h','a','t','s',' ','m','y',' ',
    'K','u','n','g',' ','F','u'};
  string expected = "5468617473206d79204b756e67204675";
  ASSERT_EQ(hex_string(kung_fu_vector), expected);
}

TEST(HexString, ByteHex){
  vector<uint8_t> byte_vector = {0x4B,0x95,0xC5,0xD3,0xAB,0x28,
    0x34,0xA9,0x18,0x7F,0xF3,0xE8,0x88,0x32,0x6E,0xBA};
  string expected = "4b95c5d3ab2834a9187ff3e888326eba";
  ASSERT_EQ(hex_string(byte_vector), expected);
}

TEST(EasyWord, Casting){
  EasyWord test = EasyWord(42);
  ASSERT_EQ(test, 42);
}

TEST(EasyWord, Assignment){
  EasyWord test = EasyWord(42);
  EasyWord test2 = 42;
  ASSERT_EQ(test, test2);
}

TEST(EasyWord, GetByte){
  EasyWord test = 2784128155;
  // 1010 0101 1111 0010 0110 1100 1001 1011

  EXPECT_EQ(test.get_byte(0), 0b10011011);
  EXPECT_EQ(test.get_byte(1), 0b01101100);
  EXPECT_EQ(test.get_byte(2), 0b11110010);
  EXPECT_EQ(test.get_byte(3), 0b10100101);
}

TEST(EasyWord, SetByte){
  EasyWord test = 2784128155;
  // 1010 0101 1111 0010 0110 1100 1001 1011

  EXPECT_EQ(test, 2784128155);
  test.set_byte(3, 0b10111011);
  EXPECT_EQ(test, 3153226907);

  test.set_byte(2, 0);
  EXPECT_EQ(test, 3137367195);

  test.set_byte(1, 0b11111111);
  EXPECT_EQ(test, 3137404827);

  test.set_byte(0, 12);
  EXPECT_EQ(test, 3137404684);

}
