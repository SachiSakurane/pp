#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/behavior.hpp>

TEST(BehaviorTest, Construction) {
  struct ClassA {
    int x;
  };
  pp::behavior<int> i{42};
  pp::behavior<std::string> s{"test string"};
  pp::behavior<ClassA> c{{.x = 42}};
}

TEST(BehaviorTest, Get) {
  pp::behavior<int> i{42};
  ASSERT_EQ(i.get(), 42);
  ASSERT_EQ(i, 42);
}

TEST(BehaviorTest, Next) {
  pp::behavior<int> i{42};
  i.next(1);
  ASSERT_EQ(i, 1);
  i = 32;
  ASSERT_EQ(i, 32);
}
