#include <iomanip>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/adapter.hpp>
#include <pp/behavior.hpp>
#include <pp/subject.hpp>

TEST(PipelineTest, HotPipeline) {
  pp::behavior<int> x{1};

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = x | [](auto i) {
    std::cout << "wa: " << i << std::endl;
    return std::to_string(i) + "<-string";
  };
  std::cout << "---" << std::endl;
  auto s = o | [](auto s) { std::cout << "terminus: " << s.c_str() << std::endl; };
  std::cout << "---" << std::endl;
  x = 42;
  ASSERT_EQ(x, 42);
  std::cout << "---" << std::endl;

  o.next(30);
  ASSERT_EQ(x, 30);

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
wa: 1
---
wa: 1
terminus: 1<-string
---
wa: 42
terminus: 42<-string
---
wa: 30
terminus: 30<-string
)");
}

TEST(PipelineTest, HotPipelineSingle) {
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
    return std::to_string(d) + "<-string";
  };

  std::cout << "---" << std::endl;

  x = 42;
  ASSERT_EQ(x, 42);

  std::cout << "---" << std::endl;

  std::cout << oo.get() << std::endl;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 2
---
int: 2
double: 3.8
---
int: 42
double: 79.8
---
int: 42
double: 79.8
79.800000<-string
)");
}

TEST(PipelineTest, HotPipelineMultiple) {
  pp::behavior<int> x{8};

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return static_cast<double>(i) * 4.64;
  } | [](double d) {
    std::cout << "double: " << d << std::endl;
    return std::to_string(d) + "<-stringx4.64";
  };

  std::cout << "---" << std::endl;

  x = 19;
  ASSERT_EQ(x, 19);

  std::cout << "---" << std::endl;

  std::cout << o.get() << std::endl;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 8
int: 8
double: 37.12
---
int: 19
double: 88.16
---
int: 19
double: 88.16
88.160000<-stringx4.64
)");
}

TEST(PipelineTest, ColdPipeline) {
  pp::subject<int> x;

  testing::internal::CaptureStdout();
  std::cout << std::endl;
  auto o = x | [](auto i) {
    std::cout << "wa: " << i << std::endl;
    return std::to_string(i) + "<-string";
  };

  auto s = o | [](auto s) { std::cout << "terminus: " << s.c_str() << std::endl; };
  x = 42;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
wa: 42
terminus: 42<-string
)");
}

TEST(PipelineTest, ColdPipelineSingle) {
  pp::subject<int> x;

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return static_cast<double>(i) * 1.9;
  };

  auto oo = o | [](double d) {
    std::cout << "double: " << d << std::endl;
    return std::to_string(d) + "<-string";
  };

  x = 42;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 42
double: 79.8
)");
}

TEST(PipelineTest, ColdPipelineMultiple) {
  pp::subject<int> x;

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto o = x | [](auto i) {
    std::cout << "int: " << i << std::endl;
    return static_cast<double>(i) * 4.64;
  } | [](double d) {
    std::cout << "double: " << d << std::endl;
    return std::to_string(d) + "<-stringx4.64";
  };

  x = 19;

  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 19
double: 88.16
)");
}
