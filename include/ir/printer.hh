#ifndef PRINTER_H_
#define PRINTER_H_

#include "ir/module.hh"

class IRPrinter {
 public:
  static void print(const ir::Module& root);
  static void print(const ir::Function& root);
  static void print(const ir::BasicBlock& root);
  static void print(const ir::Instruction& root);

 private:
  static void print_value_name(const ir::Instruction& v, std::ostream& os);
  static std::string opcode_to_string(ir::Opcode op);
};

#endif  // PRINTER_H_
