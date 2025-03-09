#include <iomanip>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/adapter.hpp>
#include <pp/subject/behavior.hpp>
#include <pp/subject/subject.hpp>
#include <pp/storage.hpp>

using namespace pp;

TEST(AdapterToObservableTest, HotSubscribe) {
  pp::behavior<int> x{42};

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  pp::hot_observer_ptr<int, std::string> o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return std::to_string(i);
  } | pp::to_observable_ptr;

  std::cout << "---" << std::endl;

  pp::storage s;

  s += *o | [](auto s) { std::cout << "sub1: " << s << std::endl; };
  s += *o | [](auto s) { std::cout << "sub2: " << s << std::endl; };

  std::cout << "---" << std::endl;

  x = 42;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(x, 42);
  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 42
int: 42
---
int: 42
sub1: 42
int: 42
sub2: 42
---
int: 42
sub1: 42
sub2: 42
)");
}
