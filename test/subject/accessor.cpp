#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <pp/subject/accessor.hpp>

TEST(SubjectAccessorTest, Construction) {
  int i = 42;
  pp::accessor<int> ai{[&]() { return i; }, [&](auto v) { i = v; }};
}

TEST(SubjectAccessorTest, Get) {
  int i = 42;
  pp::accessor<int> ai{[&]() { return i; }, [&](auto v) { i = v; }};
  ASSERT_EQ(i, 42);
  ASSERT_EQ(ai, 42);
}

TEST(SubjectAccessorTest, Next) {
  int i = 42;
  pp::accessor<int> ai{[&]() { return i; }, [&](auto v) { i = v; }};
  ai.next(1);
  ASSERT_EQ(ai, 1);
  ai = 32;
  ASSERT_EQ(ai, 32);
}

TEST(SubjectAccessorTest, Subscribe) {
  int i = 42;
  pp::accessor<int> ai{[&]() { return i; }, [&](auto v) { i = v; }};

  testing::internal::CaptureStdout();
  std::cout << std::endl;

  auto s = ai.subscribe([](auto i) { std::cout << "int: " << i << std::endl; });
  ai = 100;
  std::string log = testing::internal::GetCapturedStdout();

  ASSERT_EQ(static_cast<std::string>(log), R"(
int: 42
int: 100
)");
  ASSERT_EQ(i, 100);
}
