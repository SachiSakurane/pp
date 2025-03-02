#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <pp/adapter.hpp>
#include <pp/behavior.hpp>

TEST(Notify_ToObserverTest, AssignLeftValueObserver) {
  pp::behavior<int> b{42};
  
  auto o = b | [](auto i){ return i * 10; };
  std::unique_ptr<pp::observer<int>> oo{o | pp::to_observer_unique_ptr};
  ASSERT_EQ(oo->observe(), 420);

  b = 27;
  ASSERT_EQ(oo->observe(), 270);
}

TEST(Notify_ToObserverTest, AssignRightValueObserver) {
  pp::behavior<int> b{42};
  
  std::unique_ptr<pp::observer<int>> o{b | [](auto i){ return i * 10; } | pp::to_observer_unique_ptr};
  ASSERT_EQ(o->observe(), 420);

  b = 27;
  ASSERT_EQ(o->observe(), 270);
}
