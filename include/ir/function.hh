#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <memory>
#include <string>
#include <vector>

#include "ir/basic_block.hh"
#include "type.hh"

namespace ir {

class Function {
 public:
  std::string name;
  const Type* returnType;
  std::vector<std::unique_ptr<BasicBlock>> blocks;

  Function(std::string name, const Type* returnType)
      : name(std::move(name)), returnType(returnType) {}

  BasicBlock* createBlock(const std::string& label) {
    blocks.push_back(std::make_unique<BasicBlock>(label));
    return blocks.back().get();
  }
};

}  // namespace ir

#endif  // FUNCTION_H_
