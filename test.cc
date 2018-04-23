#include "aes.h"
#include "gtest/gtest.h"
#include <vector>

using namespace std;

TEST(EncryptSuite, ZerothRound) { EXPECT_EQ(1, 1); }

TEST(EncryptSuite, FirstRound) { EXPECT_EQ(1, 1); }

TEST(KeyMasterTest, PlaceholderTest) { EXPECT_EQ(1, 1); }
