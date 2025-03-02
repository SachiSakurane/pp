#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <pp/adapter.hpp>
#include <pp/behavior.hpp>

TEST(Notify_BehaviorTest, Construction) {
  class ClassA {};

  pp::behavior<int> i{10};
  pp::behavior<std::string> s{"string"};
  pp::behavior<ClassA> c{};
}

TEST(Notify_BehaviorTest, Cast) {
  pp::behavior<int> i{10};
  ASSERT_EQ(i, 10);

  pp::behavior<std::string> s{"string"};
  ASSERT_STREQ(static_cast<std::string>(s).c_str(), "string");
}

TEST(Notify_BehaviorTest, SubstitutionOperator) {
  {
    pp::behavior<int> i{10};
    i = 42;
    ASSERT_EQ(i, 42);
  }
  {
    pp::behavior<std::string> s{"string"};
    s = std::string{"namasute"};

    ASSERT_STREQ(static_cast<std::string>(s).c_str(), "namasute");
  }
  {
    struct ClassA {
      int x;
    };
    pp::behavior<ClassA> c{ClassA{.x = 42}};
    c = ClassA{.x = 4242};
    ASSERT_EQ(static_cast<ClassA>(c).x, 4242);
  }
}
