#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/subject.hpp>

TEST(SubjectTest, Construction) {
  struct ClassA {
    int x;
  };
  pp::subject<int> i;
  pp::subject<std::string> s;
  pp::subject<ClassA> c;
}

TEST(SubjectTest, Next) {
  pp::subject<int> i;
  i.next(1);
  i = 32;
}

TEST(SubjectTest, Subscribe) {
  pp::subject<int> i;

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto s = i.subscribe([](auto i) { std::cout << "int: " << i << std::endl; });
  i = 100;
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 100
)");
}
