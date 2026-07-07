#ifndef SOURCELOCATION_H_
#define SOURCELOCATION_H_

#include <string>
struct SourceLocation {
  int line;
  int col;

  void increment_line() { line++; }
  void increment_col() { col++; }

  std::string to_string() const {
    return std::string("line ") + std::to_string(line) + ", col " +
           std::to_string(col);
  }

  SourceLocation() : line(0), col(0) {}
};

#endif  // SOURCELOCATION_H_
