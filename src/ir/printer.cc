#include "ir/printer.hh"

#include <iostream>
#include <string>

using namespace ir;

std::string IRPrinter::opcode_to_string(Opcode op) {
  switch (op) {
    case Opcode::ConstInt:
      return "const.int";
    case Opcode::ConstBool:
      return "const.bool";
    case Opcode::GetArgument:
      return "arg";
    case Opcode::Add:
      return "add";
    case Opcode::Sub:
      return "sub";
    case Opcode::Mul:
      return "mul";
    case Opcode::Div:
      return "div";
    case Opcode::Phi:
      return "phi";
    case Opcode::Upsilon:
      return "upsilon";
    case Opcode::Jump:
      return "jump";
    case Opcode::Branch:
      return "br";
    case Opcode::Return:
      return "ret";
    default:
      return "unknown";
  }
}

// Print just the name of a value (never the whole instruction)
void IRPrinter::print_value_name(const Instruction& v, std::ostream& os) {
  if (!v.get_name().empty()) {
    os << "%" << v.get_name();
  } else {
    os << "%fuck";  // fallback
  }
}

void IRPrinter::print(const Module& mod) {
  for (const auto& f : mod.get_functions()) {
    print(*f);
    std::cout << "\n";
  }
}

void IRPrinter::print(const Function& func) {
  std::cout << "define " << func.get_return_type()->to_string() << " @"
            << func.get_name() << "() {\n";  // params later

  for (const auto& bb : func.get_blocks()) {
    print(*bb);
  }

  std::cout << "}\n";
}

void IRPrinter::print(const BasicBlock& bb) {
  std::cout << bb.get_name() << ":\n";

  for (const auto& inst : bb.get_instructions()) {
    std::cout << "  ";  // indent
    print(*inst);
  }
}

void IRPrinter::print(const Instruction& inst) {
  // Special cases first
  if (inst.get_opcode() == Opcode::Return) {
    std::cout << "ret ";
    if (!inst.get_args().empty()) {
      print_value_name(*inst.get_args()[0], std::cout);
    } else {
      std::cout << "void";
    }
    std::cout << "\n";
    return;
  }

  if (inst.get_opcode() == Opcode::Jump) {
    std::cout << "jump ";
    if (!inst.get_parent()->get_successors().empty())
      std::cout << inst.get_parent()->get_successors()[0]->get_name();
    std::cout << "\n";
    return;
  }

  if (inst.get_opcode() == Opcode::Branch) {
    std::cout << "br ";
    print_value_name(*inst.get_args()[0], std::cout);
    std::cout << ", ";
    auto suc = inst.get_parent()->get_successors();
    if (suc.size() >= 2) {
      std::cout << suc[0]->get_name() << ", " << suc[1]->get_name();
    }
    std::cout << "\n";
    return;
  }

  if (inst.get_opcode() == Opcode::Upsilon) {
    std::cout << "upsilon ";
    print_value_name(*inst.get_args()[0], std::cout);
    std::cout << ", ^";
    print_value_name(*inst.as<UpsilonInstruction>()->phi, std::cout);
    std::cout << "\n";
    return;
  }

  // Normal value-producing instruction
  print_value_name(inst, std::cout);
  std::cout << " = " << opcode_to_string(inst.get_opcode());

  if (inst.get_type()) {
    std::cout << " " << inst.get_type()->to_string();
  }

  if (!inst.get_args().empty()) {
    std::cout << " ";
    for (size_t i = 0; i < inst.get_args().size(); ++i) {
      if (i > 0) std::cout << ", ";
      print_value_name(*inst.get_args()[i], std::cout);
    }
  }

  std::cout << "\n";
}
