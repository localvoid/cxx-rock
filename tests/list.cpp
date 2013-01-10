#include <gtest/gtest.h>

#include <rock/list.h>
#include <rock/utils.h>


class MyClass {
public:
  explicit MyClass(int a=0) : i(a) {}

  void unlink() {
    list_node_.unlink();
  }

  int i;

private:
  rock::list_node list_node_;

public:
  using list_node_dmp = rock::dmp<rock::list_node MyClass::*, &MyClass::list_node_>;
};

using Container = rock::list<MyClass::list_node_dmp>;


TEST(List, push_front_one_item) {
  Container l;
  MyClass mc(1);

  l.push_front(mc);
  EXPECT_FALSE(l.empty());

  EXPECT_EQ(&l.front(), &mc);
  EXPECT_EQ(&l.back(), &mc);
}

TEST(List, push_front_two_items) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(2);

  l.push_front(mc1);
  l.push_front(mc2);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.front(), &mc2);
  EXPECT_EQ(&l.back(), &mc1);
}

TEST(List, push_back_one_item) {
  Container l;
  MyClass mc(1);

  l.push_back(mc);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.front(), &mc);
  EXPECT_EQ(&l.back(), &mc);
}

TEST(List, push_back_two_items) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(2);

  l.push_back(mc1);
  l.push_back(mc2);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.front(), &mc1);
  EXPECT_EQ(&l.back(), &mc2);
}

TEST(List, pop_front_one_item) {
  Container l;
  MyClass mc(1);

  l.push_front(mc);

  EXPECT_EQ(&l.pop_front(), &mc);
  EXPECT_TRUE(l.empty());
}

TEST(List, pop_front_two_items) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(2);

  l.push_front(mc1);
  l.push_front(mc2);

  EXPECT_EQ(&l.pop_front(), &mc2);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.pop_front(), &mc1);
  EXPECT_TRUE(l.empty());
}

TEST(List, pop_back_one_item) {
  Container l;
  MyClass mc(1);

  l.push_front(mc);

  EXPECT_EQ(&l.pop_back(), &mc);
  EXPECT_TRUE(l.empty());
}

TEST(List, pop_back_two_items) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(1);

  l.push_front(mc1);
  l.push_front(mc2);

  EXPECT_EQ(&l.pop_back(), &mc1);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.pop_back(), &mc2);
  EXPECT_TRUE(l.empty());
}

TEST(List, erase) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(1);

  l.push_front(mc1);
  l.push_front(mc2);

  l.erase(mc2);
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.front(), &mc1);
  EXPECT_EQ(&l.back(), &mc1);

  l.erase(mc1);
  EXPECT_TRUE(l.empty());
}

TEST(List, unlink) {
  Container l;
  MyClass mc1(1);
  MyClass mc2(1);

  l.push_front(mc1);
  l.push_front(mc2);

  mc2.unlink();
  EXPECT_FALSE(l.empty());
  EXPECT_EQ(&l.front(), &mc1);
  EXPECT_EQ(&l.back(), &mc1);

  mc1.unlink();
  EXPECT_TRUE(l.empty());
}
