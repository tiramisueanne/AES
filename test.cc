#include <vector>
#include <string>
#include <iostream>
#include <bitset>
#include "gtest/gtest.h"
#include "aes.h"

using namespace std;

TEST(StringToByteVectorTest, CharCompare) {
  string sample = "Thats my Kung Fu";
  vector<uint8_t> from_string = string_to_byte_vector(sample);
  for(int i = 0; i<from_string.size(); i++){
    ASSERT_EQ(from_string[i], sample[i]);
  }
}

TEST(StringToByteVectorTest, CStrCompare) {
  char const* sample = "Thats my Kung Fu";
  vector<uint8_t> from_string = string_to_byte_vector(string(sample));
  from_string.push_back('\0');
  ASSERT_STREQ((char const*)from_string.data(), sample);
}

TEST(HexStringTest, CharHex){
  vector<uint8_t> kung_fu_vector = {'T','h','a','t','s',' ','m','y',' ',
    'K','u','n','g',' ','F','u'};
  string expected = "5468617473206d79204b756e67204675";
  ASSERT_EQ(hex_string(kung_fu_vector), expected);
}

TEST(HexStringTest, ByteHex){
  vector<uint8_t> byte_vector = {0x4B,0x95,0xC5,0xD3,0xAB,0x28,
    0x34,0xA9,0x18,0x7F,0xF3,0xE8,0x88,0x32,0x6E,0xBA};
  string expected = "4b95c5d3ab2834a9187ff3e888326eba";
  ASSERT_EQ(hex_string(byte_vector), expected);
}

TEST(EasyWordTest, Casting){
  EasyWord test = EasyWord(42);
  ASSERT_EQ(test, 42);
}

TEST(EasyWordTest, Assignment){
  EasyWord test = EasyWord(42);
  EasyWord test2 = 42;
  ASSERT_EQ(test, test2);
}

TEST(EasyWordTest, GetByte){
  EasyWord test = 2784128155;
  // 1010 0101 1111 0010 0110 1100 1001 1011

  EXPECT_EQ(test.get_byte(3), 0b10011011);
  EXPECT_EQ(test.get_byte(2), 0b01101100);
  EXPECT_EQ(test.get_byte(1), 0b11110010);
  EXPECT_EQ(test.get_byte(0), 0b10100101);
}

TEST(EasyWordTest, SetByte){
  EasyWord test = 2784128155;
  // 1010 0101 1111 0010 0110 1100 1001 1011

  EXPECT_EQ(test, 2784128155);
  test.set_byte(0, 0b10111011);
  EXPECT_EQ(test, 3153226907);

  test.set_byte(1, 0);
  EXPECT_EQ(test, 3137367195);

  test.set_byte(2, 0b11111111);
  EXPECT_EQ(test, 3137404827);

  test.set_byte(3, 12);
  EXPECT_EQ(test, 3137404684);

}

struct KeyMasterTest : public testing::Test {
  const vector<uint8_t> kung_fu_vector = {'T','h','a','t','s',
  ' ','m','y',' ','K','u','n','g',' ','F','u'};
  KeyMaster* master;
  void SetUp() {
    master = new KeyMaster(kung_fu_vector);
  }
  void TearDown(){}
};

TEST_F(KeyMasterTest, Init){
  ASSERT_EQ(master->get_next_word(), 0x54686174);
  ASSERT_EQ(master->get_next_word(), 0x73206D79);
  ASSERT_EQ(master->get_next_word(), 0x204B756E);
  ASSERT_EQ(master->get_next_word(), 0x67204675);
}

TEST_F(KeyMasterTest, NumRounds){
  ASSERT_EQ(master->get_num_rounds(), 10);
  const vector<uint8_t> long_vector (32, 42);
  KeyMaster* longmaster = new KeyMaster(long_vector);
  ASSERT_EQ(longmaster->get_num_rounds(), 14);
}

TEST_F(KeyMasterTest, RoundKeys128Bit){
  for(int i = 0; i < 4*(master->get_num_rounds()); i++){
    master->get_next_word();
  }
  ASSERT_EQ(master->get_next_word(), 0x28FDDEF8);
  ASSERT_EQ(master->get_next_word(), 0x6DA4244A);
  ASSERT_EQ(master->get_next_word(), 0xCCC0A4FE);
  ASSERT_EQ(master->get_next_word(), 0x3B316F26);
}

TEST_F(KeyMasterTest, RoundKeys256Bit){
  const vector<uint8_t> long_vector = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,
  0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,
  0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
  KeyMaster* longmaster = new KeyMaster(long_vector);
  for(int i = 0; i < 4*(longmaster->get_num_rounds()); i++){
    longmaster->get_next_word();
  }
  ASSERT_EQ(longmaster->get_next_word(), 0xFE4890D1);
  ASSERT_EQ(longmaster->get_next_word(), 0xE6188D0B);
  ASSERT_EQ(longmaster->get_next_word(), 0x046DF344);
  ASSERT_EQ(longmaster->get_next_word(), 0x706C631E);
}

