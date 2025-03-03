#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <pp/adapter.hpp>
#include <pp/behavior.hpp>
#include <pp/storage.hpp>

// TEST(Notify_StorageTest, Pipeline) {
//   pp::storage s;
//   pp::behavior<int> i{10};

//   testing::internal::CaptureStdout();
//   s += i | [](auto i) {
//     std::cout << "multiple 100" << std::endl;
//     return i * 100;
//   } | [](auto i) { std::cout << "log: " << i << std::endl; };
//   i = 42;
//   std::string log = testing::internal::GetCapturedStdout();
//   ASSERT_EQ(static_cast<std::string>(log), "log: 1000\nlog: 4200\n");
// }

// TEST(Notify_StorageTest, LifetimeExtend) {
//   pp::storage s;
//   pp::behavior<int> i{1};

//   {
//     auto discarded = i | [](auto i) { std::cout << "discarded log: " << i << std::endl; };
//     s += i | [](auto i) { std::cout << "extended: " << i << ","; };
//     s += i | [](auto i) { return i * 100; } |
//          [](auto i) { std::cout << "extended: " << i << std::endl; };
//   }

//   testing::internal::CaptureStdout();
//   i = 42;
//   std::string log = testing::internal::GetCapturedStdout();
//   ASSERT_EQ(static_cast<std::string>(log), "extended: 42,extended: 4200\n");
// }
