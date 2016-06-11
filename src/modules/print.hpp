#ifndef PRINT_HPP
#define PRINT_HPP

#include <cstdio>

// putchard - putchar that takes a double and returns 0.
extern "C" double putchard(double x) {
  fputc((char)x, stderr);
  return 0;
}

// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" double printd(double x) {
  fprintf(stderr, "%f\n", x);
  return 0;
}

#endif
