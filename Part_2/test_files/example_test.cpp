// Copyright (c) 2022, The MaPra Authors.

#include <algorithm>
#include <iostream>
#include <string>

#include "../include/mapra_test.h"

int main()
{
  mapra::MapraTest test("Beispiel");
  test.Assert("cin works", std::cin.good());
  test.AssertEq("max(-4,3) is 3", 3, std::max(-4, 3));
  test.AssertEq("max(3,-4) is 3", 2, std::max(3, -4)); // extra falsch
  return 0;
}
