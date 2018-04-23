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

TEST(EasyWordTest, FourByteConstructor) {
    int x = 300;
    EasyWord tester(x >> 24, (x << 8) >> 24, (x << 16) >> 24, (x << 24) >> 24);
    ASSERT_EQ(tester, 300);
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
    //cout << hex<< longmaster->get_next_word() << endl;
  }
  ASSERT_EQ(longmaster->get_next_word(), 0xBD10190D);
  ASSERT_EQ(longmaster->get_next_word(), 0xFE4890D1);
  ASSERT_EQ(longmaster->get_next_word(), 0xE6188D0B);
  ASSERT_EQ(longmaster->get_next_word(), 0x046DF344);
}

struct AesTest : testing::Test {
    AES *machine;
    //TODO: This is buggy
    static vector<uint8_t> string_hex_to_bytes(string _hex) {
        vector<uint8_t> bytes_;
        for(int i = 0; i <  _hex.size() -1; i+=2) {
           string hex_ = "";
           hex_ += _hex[i];
           hex_ += _hex[i+1];
           stringstream ss;
           ss << hex << hex_;
           unsigned char x;
           ss >> x;
           bytes_.emplace_back(x);
           cout << hex << x << " ";
        }
        return bytes_;
    }
    static vector<EasyWord> bytes_to_words(vector<uint8_t> bytes_) {
        /*
        vector<EasyWord> _words;
        for(int i = 0; i <
        */
        return vector<EasyWord>();
    }
    virtual void SetUp() {
        string FIPS_128 = "00112233445566778899aabbccddeeff";
        vector<uint8_t> key_128 = string_hex_to_bytes(FIPS_128); 
        machine = new AES(key_128);
    }

    virtual void TearDown() {};
};

TEST_F(AesTest, InitState) {
    vector<uint8_t> bytes_ = AesTest::string_hex_to_bytes("00112233445566778899aabbccddeeff"); 
    //check and see if state is correct
    for(int col = 0; col < 4; col++) {
        for(int row  = 0; row < 4; row++) {
            ASSERT_EQ(machine->state_[row][col], bytes_[row + col*4]); 
        }
    }
}

TEST_F(AesTest, InitKey) { 
    /*
vector<uint8_t> key_sch = AesTest::string_hex_to_bytes("000102030405060708090a0b0c0d0e0f");
    EasyWord next_ = machine->master_.get_next_word();
    for(int i = 0; i < 4; i++) {
        ASSERT_EQ(next_, key_[i]); 
    }
    */
}

