#include <gtest/gtest.h>

#include <rock/chain.hpp>
#include <rock/utils.hpp>


class MyClass {
public:
  int i;

private:
  rock::chain_node chain_node_;

public:
  using chain_node_dmp = rock::dmp<rock::chain_node MyClass::*, &MyClass::chain_node_>;
};

using Container = rock::chain<MyClass::chain_node_dmp>;

class EmptyChain : public ::testing::Test {
protected:
  Container c;
};

class OneElement : public ::testing::Test {
protected:
  virtual void SetUp() {
    mc.i = 1;
    c.push(mc);
  }

  Container c;
  MyClass mc;
};

class TwoElements : public ::testing::Test {
protected:
  virtual void SetUp() {
    mc1.i = 1;
    mc2.i = 2;
    c.push(mc1);
    c.push(mc2);
  }

  Container c;
  MyClass mc1;
  MyClass mc2;
};


TEST_F(EmptyChain, empty) {
  EXPECT_TRUE(c.empty());
}

TEST_F(EmptyChain, iterators) {
  auto a = std::begin(c);
  auto b = std::end(c);
  EXPECT_EQ(a, b);
}

TEST_F(OneElement, empty) {
  EXPECT_FALSE(c.empty());
}

TEST_F(OneElement, front) {
  EXPECT_EQ(&mc, &c.front());
}

TEST_F(OneElement, pop) {
  EXPECT_EQ(&mc, &c.pop());
  EXPECT_TRUE(c.empty());
}

TEST_F(OneElement, erase) {
  c.erase(mc);
  EXPECT_TRUE(c.empty());
}

TEST_F(OneElement, iterators) {
  auto c_begin = std::begin(c);
  auto c_end = std::end(c);
  EXPECT_EQ(c_begin->i, 1);
  ++c_begin;
  EXPECT_EQ(c_begin, c_end);
}

TEST_F(TwoElements, empty) {
  EXPECT_FALSE(c.empty());
}

TEST_F(TwoElements, front) {
  EXPECT_EQ(&mc2, &c.front());
}

TEST_F(TwoElements, pop) {
  EXPECT_EQ(&mc2, &c.pop());
  EXPECT_FALSE(c.empty());

  EXPECT_EQ(&mc1, &c.pop());
  EXPECT_TRUE(c.empty());
}

TEST_F(TwoElements, erase) {
  c.erase(mc2);
  EXPECT_FALSE(c.empty());
  EXPECT_EQ(&mc1, &c.front());

  c.erase(mc1);
  EXPECT_TRUE(c.empty());
}

TEST_F(TwoElements, iterators) {
  auto c_begin = std::begin(c);
  auto c_end = std::end(c);
  EXPECT_EQ(c_begin->i, 2);
  ++c_begin;
  EXPECT_EQ(c_begin->i, 1);
  ++c_begin;
  EXPECT_EQ(c_begin, c_end);
}

TEST_F(TwoElements, foreach) {
  int i = 2;
  for (auto &a: c) {
    EXPECT_EQ(a.i, i--);
  }
}
