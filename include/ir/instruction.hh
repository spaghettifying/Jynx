#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "type.hh"

namespace ir {

class BasicBlock;

enum Opcode : uint16_t {
  ConstInt,
  ConstBool,

  Add,
  Sub,
  Mul,
  Div,

  GetArgument,

  Phi,
  Upsilon,

  Jump,
  Branch,
  Return,
};

/// IR Instructions have a opcode, a type and optionally args (think 'add %a,
/// %b', '%a' and '%b' are the args), and a parent basic block. Only
/// instructions that need extra information have subclasses for now.
class Instruction {
 public:
  Instruction(Opcode op, const Type* type) : opcode(op), type(type) {}
  virtual ~Instruction() = default;

  bool is_terminator() const {
    return opcode == Opcode::Jump || opcode == Opcode::Branch ||
           opcode == Opcode::Return;
  }

  template <typename T>
  T* as() {
    return static_cast<T*>(this);
  }

  template <typename T>
  const T* as() const {
    return static_cast<const T*>(this);
  }

  void set_parent(BasicBlock* parent) { this->parent = parent; }
  const BasicBlock* get_parent() const { return parent; }

  Opcode get_opcode() const { return opcode; }

  const Type* get_type() const { return type; }

  void set_args(std::vector<Instruction*> args) { this->args = args; }
  const std::vector<Instruction*>& get_args() const { return args; }

  void set_name(std::string name) { this->name = name; }
  const std::string get_name() const { return name; }

 protected:
  Opcode opcode;
  const Type* type = nullptr;
  std::vector<Instruction*> args;
  BasicBlock* parent = nullptr;
  std::string name;
};

class ConstInt : public Instruction {
 public:
  int64_t value;
  ConstInt(const Type* type, int64_t v)
      : Instruction(Opcode::ConstInt, type), value(v) {}
};

class ConstBool : public Instruction {
 public:
  bool value;
  ConstBool(const Type* type, bool v)
      : Instruction(Opcode::ConstBool, type), value(v) {}
};

class GetArgumentInstruction : public Instruction {
 public:
  unsigned arg_index;
  GetArgumentInstruction(const Type* type, unsigned idx)
      : Instruction(Opcode::GetArgument, type), arg_index(idx) {}
};

class UpsilonInstruction : public Instruction {
 public:
  Instruction* phi = nullptr;
  UpsilonInstruction(Instruction* value, Instruction* target_phi)
      : Instruction(Opcode::Upsilon, nullptr), phi(target_phi) {
    args.push_back(value);
  }
};

class PhiInstruction : public Instruction {
 public:
  explicit PhiInstruction(const Type* type) : Instruction(Opcode::Phi, type) {}
};

}  // namespace ir

#endif  // INSTRUCTION_H_
