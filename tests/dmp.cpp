#include <gtest/gtest.h>

#include <rock/utils.hpp>

namespace {

class DMPTest {
public:
  DMPTest() {}

public:
  int a_ = 0x222222;
  int padding_ = 0xDEADBEEF;
  int b_ = 0x333333;

public:
  using b_dmp = rock::dmp<int DMPTest::*, &DMPTest::b_>;
};

}

TEST(DMPTest, to_member) {
  ::DMPTest x;

  int *b = ::DMPTest::b_dmp::to_member(&x);
  EXPECT_EQ(0x333333, *b);
}

TEST(DMPTest, to_container) {
  ::DMPTest x;

  ::DMPTest *x2 = ::DMPTest::b_dmp::to_container(&x.b_);
  EXPECT_EQ(x2, &x);
}
