#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <memory>
#include <string>
#include <vector>

#include "ir/basic_block.hh"
#include "type.hh"

namespace ir {
/// IR Functions have a list of basic blocks
class Function {
 public:
  Function(std::string name, const Type* ret_type)
      : name(std::move(name)), return_type(ret_type) {}

  BasicBlock* entry() { return blocks.empty() ? nullptr : blocks[0].get(); }

  BasicBlock* create_block(std::string name) {
    auto bb = std::make_unique<BasicBlock>(std::move(name));
    bb->set_parent(this);
    BasicBlock* raw = bb.get();
    blocks.push_back(std::move(bb));
    return raw;
  }

  const Type* get_return_type() const { return return_type; }
  const std::string get_name() const { return name; }
  const std::vector<std::unique_ptr<BasicBlock>>& get_blocks() const {
    return blocks;
  }

 private:
  std::string name;
  const Type* return_type = nullptr;
  std::vector<std::unique_ptr<BasicBlock>> blocks;
};
}  // namespace ir

#endif  // FUNCTION_H_
