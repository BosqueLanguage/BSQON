#include "smt_utils.h"
#include <cstdio>
#include <iostream>

int main() {
  z3::context c;
  z3::solver s(c);

  z3::expr int1 = c.int_val(1);
  z3::expr int2 = c.int_val(2);
  s.add(int2 > 1);

  std::cout << s.check() << "\n";
}
