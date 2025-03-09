#include <iostream>

#include <gtest/gtest.h>

#include <pp/adapter/pipeline.hpp>
#include <pp/adapter/subscribe.hpp>
#include <pp/subject/behavior.hpp>

TEST(AdapterSubscribeTest, HotSubscribe) {
  pp::behavior<int> x{1};

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = x | [](auto i) { std::cout << "wa: " << i << std::endl; };
  std::cout << "---" << std::endl;
  x = 42;
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(x, 42);
  ASSERT_EQ(static_cast<std::string>(log), R"(
wa: 1
---
wa: 42
)");
}

TEST(AdapterSubscribeTest, HotPipelineSingle) {
  pp::behavior<int> x{2};

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return static_cast<double>(i) * 1.9;
  };

  std::cout << "---" << std::endl;

  auto oo = o | [](double d) {
    std::cout << "double: " << d << std::endl;
  };

  std::cout << "---" << std::endl;

  x = 42;
  ASSERT_EQ(x, 42);

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 2
---
int: 2
double: 3.8
---
int: 42
double: 79.8
)");
}

TEST(AdapterSubscribeTest, HotPipelineMultiple) {
  pp::behavior<int> x{8};

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return static_cast<double>(i) * 4.64;
  } | [](double d) {
    std::cout << "double: " << d << std::endl;
  };

  std::cout << "---" << std::endl;

  x = 19;
  ASSERT_EQ(x, 19);

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 8
int: 8
double: 37.12
---
int: 19
double: 88.16
)");
}
