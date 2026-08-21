#ifndef BASIC_BLOCK_H_
#define BASIC_BLOCK_H_

#include <memory>
#include <vector>

#include "ir/instruction.hh"

namespace ir {

class Function;

/// IR Basic Blocks have a list of instructions, and keep a list of successing
/// blocks
class BasicBlock {
 public:
  explicit BasicBlock(std::string name) : name(std::move(name)) {}

  Instruction* terminator() const {
    if (instructions.empty()) return nullptr;
    Instruction* last = instructions.back().get();
    return last->is_terminator() ? last : nullptr;
  }

  void append(std::unique_ptr<Instruction> instruction) {
    instruction->set_parent(this);
    instructions.push_back(std::move(instruction));
  }

  void set_parent(Function* parent) { this->parent = parent; }
  const std::string get_name() const { return name; }

  const std::vector<std::unique_ptr<Instruction>>& get_instructions() const {
    return instructions;
  }

  void set_successors(std::vector<BasicBlock*> successors) {
    this->successors = successors;
  }
  const std::vector<BasicBlock*>& get_successors() const { return successors; }
  void add_successor(BasicBlock* successor) {
    this->successors.push_back(successor);
  }

 private:
  std::string name;
  std::vector<std::unique_ptr<Instruction>> instructions;
  std::vector<BasicBlock*> successors;
  Function* parent = nullptr;
};

}  // namespace ir

#endif  // BASIC_BLOCK_H_
