#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <pp/adapter.hpp>
#include <pp/behavior.hpp>

TEST(BehaviorTest, Construction) {
  class ClassA {};

  pp::behavior<int> i{10};
  pp::behavior<std::string> s{"string"};
  pp::behavior<ClassA> c{};
}

TEST(BehaviorTest, CastExpression) {
  pp::behavior<int> i{10};
  ASSERT_EQ(i, 10);

  pp::behavior<std::string> s{"string"};
  ASSERT_STREQ(static_cast<std::string>(s).c_str(), "string");
}

TEST(BehaviorTest, AssignmentOperator) {
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

TEST(BehaviorTest, NotifiedWhenValuesAreUpdated) {
  pp::behavior<int> i{42};

  ASSERT_EQ(i, 42);

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = i | [](auto i) {
    std::cout << "to string" << std::endl;
    return std::to_string(i);
  };
  auto s = o | [](auto s) { std::cout << "string: " << s << std::endl; };
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
to string
to string
string: 42
)");

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  i = 4242;
  log = testing::internal::GetCapturedStdout();
  ASSERT_EQ(static_cast<std::string>(log), R"(
to string
string: 4242
)");
}
