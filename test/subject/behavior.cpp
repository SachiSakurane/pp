#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/subject/behavior.hpp>

TEST(SubjectBehaviorTest, Construction) {
  struct ClassA {
    int x;
  };
  pp::behavior<int> i{42};
  pp::behavior<std::string> s{"test string"};
  pp::behavior<ClassA> c{{.x = 42}};
}

TEST(SubjectBehaviorTest, Get) {
  pp::behavior<int> i{42};
  ASSERT_EQ(i.get(), 42);
  ASSERT_EQ(i, 42);
}

TEST(SubjectBehaviorTest, Next) {
  pp::behavior<int> i{42};
  i.next(1);
  ASSERT_EQ(i, 1);
  i = 32;
  ASSERT_EQ(i, 32);
}

TEST(SubjectBehaviorTest, Subscribe) {
  pp::behavior<int> i{42};

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto s = i.subscribe([](auto i) { std::cout << "int: " << i << std::endl; });
  i = 100;
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 42
int: 100
)");
}
