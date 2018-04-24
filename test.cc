#include <vector>
#include <string>
#include <iostream>
#include <bitset>
#include <cstdint>
#include "gtest/gtest.h"
#include "aes.h"

using namespace std;
#define FIX_256

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
  void TearDown(){};
};

TEST_F(KeyMasterTest, Init){
  ASSERT_EQ(master->get_next_word(), 0x54686174);
  ASSERT_EQ(master->get_next_word(), 0x73206D79);
  ASSERT_EQ(master->get_next_word(), 0x204B756E);
  ASSERT_EQ(master->get_next_word(), 0x67204675);
}

TEST_F(KeyMasterTest, NumRounds){
  //Test the two different size
  EXPECT_EQ(master->get_num_rounds(), 10);

#ifdef FIX_256
  const vector<uint8_t> long_vector (32, 42);
  KeyMaster* longmaster = new KeyMaster(long_vector);
  EXPECT_EQ(longmaster->get_num_rounds(), 14);
#endif

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

#ifdef FIX_256
TEST_F(KeyMasterTest, RoundKeys256Bit){
  const vector<uint8_t> long_vector = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,
  0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,
  0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
  KeyMaster* longmaster = new KeyMaster(long_vector);
  for(int i = 0; i < 4*(longmaster->get_num_rounds()); i++){
    longmaster->get_next_word();
  }
  EXPECT_EQ(longmaster->get_next_word(), 0xFE4890D1);
  EXPECT_EQ(longmaster->get_next_word(), 0xE6188D0B);
  EXPECT_EQ(longmaster->get_next_word(), 0x046DF344);
  EXPECT_EQ(longmaster->get_next_word(), 0x706C631E);
}
#endif

struct AESTest128 : testing::Test {
    void check_vector_state(vector<uint8_t> vec) {
        for(int col = 0; col < 4; col++) {
            for(int row = 0; row < 4; row++) {
                ASSERT_EQ(machine->state_[row][col], vec[row + col*4]);
            }
        }
    }

    static vector<uint8_t> string_hex_to_bytes(string _hex) {
        vector<uint8_t> bytes_;
        for(int i = 0; i <  _hex.size() -1; i+=2) {
           string hex_ = "";
           hex_ += {_hex[i], _hex[i+1]};
           //cout << "hex_" << hex_ << endl;
           stringstream ss;
           ss << hex_;
           int x;
           ss >> hex >> x;
           uint8_t to_place = x;
           bytes_.emplace_back(to_place);
        }
        return bytes_;
    }

    AES *machine;
    string plaintext = "00112233445566778899aabbccddeeff";
    vector<uint8_t> text_ = string_hex_to_bytes(plaintext);
    //TODO: This is buggy

    static vector<EasyWord> bytes_to_words(vector<uint8_t> bytes_) {
        /*
        vector<EasyWord> _words;
        for(int i = 0; i <
        */
        return vector<EasyWord>();
    }
    virtual void SetUp() {
        string FIPS_128 = "000102030405060708090a0b0c0d0e0f";
        vector<uint8_t> key_128 = string_hex_to_bytes(FIPS_128);
        machine = new AES(key_128);
    }

    virtual void TearDown() {
        //delete machine;
    };
};

TEST_F(AESTest128, InitState) {
    machine->input_to_state(text_);
    //check and see if state is correct
    for(int col = 0; col < 4; col++) {
        for(int row  = 0; row < 4; row++) {
            EXPECT_EQ(machine->state_[row][col], text_[row + col*4]);
        }
    }
}

TEST_F(AESTest128, InitKey) {
    vector<uint8_t> key_sch = AESTest128::string_hex_to_bytes("000102030405060708090a0b0c0d0e0f");
    EasyWord next_word = machine->master_.get_next_word();
    //Tried indexing descending and ascending and both look wrong sadly
    EasyWord correct_word = EasyWord(key_sch[0], key_sch[1], key_sch[2], key_sch[3]);
    EXPECT_EQ(next_word, correct_word);
}

TEST_F(AESTest128, ShiftRows) {
    uint8_t state[4][4] = {
      {0x0, 0x1, 0x2, 0x3},
      {0x4, 0x5, 0x6, 0x7},
      {0x8, 0x9, 0xA, 0xB},
      {0xC, 0xD, 0xE, 0xF}
    };
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        machine->state_[i][j] = state[i][j];
      }
    }
    machine->shift_rows();

    // see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
    //    (p.17, 5.1.2)
    uint8_t expected_state[4][4] = {
      {0x0, 0x1, 0x2, 0x3},
      {0x5, 0x6, 0x7, 0x4},
      {0xA, 0xB, 0x8, 0x9},
      {0xF, 0xC, 0xD, 0xE}
    };
    //check and see if state is correct
    for(int col = 0; col < 4; col++) {
        for(int row  = 0; row < 4; row++) {
            EXPECT_EQ(machine->state_[row][col], expected_state[row][col]);
        }
    }
}

TEST_F(AESTest128, InvShiftRows) {
    uint8_t state[4][4] = {
      {0x0, 0x1, 0x2, 0x3},
      {0x4, 0x5, 0x6, 0x7},
      {0x8, 0x9, 0xA, 0xB},
      {0xC, 0xD, 0xE, 0xF}
    };
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        machine->state_[i][j] = state[i][j];
      }
    }
    machine->inv_shift_rows();

    // see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
    //    (p.17, 5.1.2)
    uint8_t expected_state[4][4] = {
      {0x0, 0x1, 0x2, 0x3},
      {0x7, 0x4, 0x5, 0x6},
      {0xA, 0xB, 0x8, 0x9},
      {0xD, 0xE, 0xF, 0xC}
    };
    //check and see if state is correct
    for(int col = 0; col < 4; col++) {
        for(int row  = 0; row < 4; row++) {
            EXPECT_EQ(machine->state_[row][col], expected_state[row][col]);
        }
    }
}

TEST_F(AESTest128, ShiftInvShiftRows) {
    uint8_t state[4][4] = {
      {0x0, 0x1, 0x2, 0x3},
      {0x4, 0x5, 0x6, 0x7},
      {0x8, 0x9, 0xA, 0xB},
      {0xC, 0xD, 0xE, 0xF}
    };
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        machine->state_[i][j] = state[i][j];
      }
    }
    machine->inv_shift_rows();
    machine->shift_rows();
    //check and see if state is correct
    for(int col = 0; col < 4; col++) {
        for(int row  = 0; row < 4; row++) {
            EXPECT_EQ(machine->state_[row][col], state[row][col]);
        }
    }
}

TEST_F(AESTest128, MixColumn) {
    uint8_t new_state[4][4] = {
      {0xd4, 0, 0, 0},
      {0xbf, 0, 0, 0},
      {0x5d, 0, 0, 0},
      {0x30, 0, 0, 0}
    };
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        machine->state_[i][j] = new_state[i][j];
      }
    }

    vector<uint8_t> correct_col = {0x04, 0x66, 0x81, 0xe5};
    function<uint8_t(uint8_t)> multiply_1 = [](uint8_t x) { return x; };
    function<uint8_t(uint8_t)> multiply_2 = [](uint8_t x) {
        return AES::mult_2[(int)x];
    };
    function<uint8_t(uint8_t)> multiply_3 = [](uint8_t x) {
        return AES::mult_3[(int)x];
    };

    //this is the mix_Column matrix
    function<uint8_t(uint8_t)> mult_by_mat[4][4] = {
      {multiply_2, multiply_3, multiply_1, multiply_1},
      {multiply_1, multiply_2, multiply_3, multiply_1},
      {multiply_1, multiply_1, multiply_2, multiply_3},
      {multiply_3, multiply_1, multiply_1, multiply_2}};
    vector<uint8_t> check = machine->multiply_column(mult_by_mat, 0);
    for(int i = 0; i < 4; i++) {
        EXPECT_EQ(check[i], correct_col[i]);
    }
}

TEST_F(AESTest128, BasicSubBytes) {
  uint8_t state[4][4] = {
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0}
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      machine->state_[i][j] = state[i][j];
    }
  }

  machine->sub_bytes();

  for(int col = 0; col < 4; col++) {
      for(int row  = 0; row < 4; row++) {
          EXPECT_EQ(machine->state_[row][col], 0x63);
      }
  }
}

TEST_F(AESTest128, SubBytes) {

  uint8_t state[4][4] = {
    {0xA6, 0x4F, 0x4B, 0x34},
    {0x79, 0x04, 0x5F, 0x14},
    {0xFA, 0x56, 0xB0, 0x20},
    {0x5F, 0x0A, 0x54, 0xD2}
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      machine->state_[i][j] = state[i][j];
    }
  }

  machine->sub_bytes();

  // calculated by hand using S-box figure in
  // https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
  //    (p.16, 5.1.1)
  uint8_t expected_state[4][4] = {
    {0x24, 0x84, 0xB3, 0x18},
    {0xB6, 0xF2, 0xCF, 0xFA},
    {0x2D, 0xB1, 0xE7, 0xB7},
    {0xCF, 0x67, 0x20, 0xB5}
  };

  for(int col = 0; col < 4; col++) {
      for(int row  = 0; row < 4; row++) {
          EXPECT_EQ(machine->state_[row][col], expected_state[row][col]);
      }
  }
}

TEST_F(AESTest128, BasicInvSubBytes) {
  uint8_t state[4][4] = {
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0}
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      machine->state_[i][j] = state[i][j];
    }
  }

  machine->inv_sub_bytes();

  for(int col = 0; col < 4; col++) {
      for(int row  = 0; row < 4; row++) {
          EXPECT_EQ(machine->state_[row][col], 0x52);
      }
  }
}

TEST_F(AESTest128, InvSubBytes) {

  uint8_t state[4][4] = {
    {0x58, 0xEB, 0x80, 0xAE},
    {0x25, 0x15, 0xF1, 0xFE},
    {0xF9, 0x5C, 0xAB, 0x83},
    {0xED, 0x11, 0x99, 0x54}
  };
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      machine->state_[i][j] = state[i][j];
    }
  }

  machine->inv_sub_bytes();

  // calculated by hand using S-box figure in
  // https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
  //    (p.16, 5.1.1)
  uint8_t expected_state[4][4] = {
    {0x5E, 0x3C, 0x3A, 0xBE},
    {0xC2, 0x2F, 0x2B, 0x0C},
    {0x69, 0xA7, 0x0E, 0x41},
    {0x53, 0xE3, 0xF9, 0xFD}
  };

  for(int col = 0; col < 4; col++) {
      for(int row  = 0; row < 4; row++) {
          EXPECT_EQ(machine->state_[row][col], expected_state[row][col]);
      }
  }
}

TEST_F(AESTest128, FirstRound) {
    vector<uint8_t> start_ = AESTest128::string_hex_to_bytes("00102030405060708090a0b0c0d0e0f0");
    machine->input_to_state(text_);
    machine->add_round_key();
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            ASSERT_EQ(machine->state_[row][col], start_[row + col*4]);
        }
    }
    vector<uint8_t> s_box = string_hex_to_bytes("63cab7040953d051cd60e0e7ba70e18c");
    ASSERT_EQ(s_box.size(), 16);
    machine->sub_bytes();
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            ASSERT_EQ(machine->state_[row][col], s_box[row + col*4]);
        }
    }

    machine->shift_rows();
    vector<uint8_t> shift_row = string_hex_to_bytes("6353e08c0960e104cd70b751bacad0e7");
    check_vector_state(shift_row);

    machine->mix_columns();
    vector<uint8_t> mix_columns = string_hex_to_bytes("5f72641557f5bc92f7be3b291db9f91a");
    check_vector_state(mix_columns);

    machine->add_round_key();

    //checking the start of the next thing, which is after checking round_key
    vector<uint8_t> end_first = string_hex_to_bytes("89d810e8855ace682d1843d8cb128fe4");
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            ASSERT_EQ(machine->state_[row][col], end_first[row + col*4]);
        }
    }

}

//So Encrypt doesn't work all the way
TEST_F(AESTest128, FullEncrypt) {
    vector<uint8_t> end_encrypt = AESTest128::string_hex_to_bytes("69c4e0d86a7b0430d8cdb78070b4c55a");
    vector<uint8_t> encrypted = machine->encrypt_this(text_);
    for(int i = 0; i < 4; i++) {
        EXPECT_EQ(end_encrypt[i], encrypted[i]);
    }
}
