#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <pp/adapter.hpp>
#include <pp/subject.hpp>

TEST(SubjectTest, Construction) {
  class ClassA {};

  pp::subject<int> i;
  pp::subject<std::string> s;
  pp::subject<ClassA> c;
}

TEST(SubjectTest, ThrowExpressionNoInitialValueObserve) {
  pp::subject<int> i;
  ASSERT_THROW(i.observe();, std::bad_optional_access);
}

TEST(SubjectTest, AssignmentOperator) {
  pp::subject<int> i;
  i = 10;
  ASSERT_EQ(i.observe(), 10);

  pp::subject<std::string> s;
  s = "string";
  ASSERT_STREQ(s.observe().c_str(), "string");

  struct ClassA {
    int x;
  };
  pp::subject<ClassA> c;
  c = ClassA{.x = 42};
  ASSERT_EQ(c.observe().x, 42);
}

TEST(SubjectTest, NotifiedWhenValuesAreUpdated) {
  pp::subject<int> i;

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = i | [](auto i) {
    std::cout << "to string" << std::endl;
    return std::to_string(i);
  };
  auto s = o | [](auto s) { std::cout << "string: " << s << std::endl; };
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
)");

//   testing::internal::CaptureStdout();
//   std::cout << std::endl;
//   i = 42;
//   log = testing::internal::GetCapturedStdout();
//   ASSERT_EQ(static_cast<std::string>(log), R"(
// to string
// string: 42
// )");
}
