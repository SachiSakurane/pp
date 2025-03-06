#include <iostream>

#include <gtest/gtest.h>

#include <pp/adapter/subscribe.hpp>
#include <pp/behavior.hpp>

TEST(AdapterSubscribeTest, HotPipeline) {
  pp::behavior<int> x{1};

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = x | [](auto i) { std::cout << "wa: " << i << std::endl; };
  x = 42;
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(x, 42);
  ASSERT_EQ(static_cast<std::string>(log), R"(
wa: 1
wa: 42
)");
}
