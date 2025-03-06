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
