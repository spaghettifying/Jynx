#ifndef MODULE_H_
#define MODULE_H_

#include <memory>
#include <vector>

#include "ir/function.hh"

namespace ir {
/// IR Module has a list of functions (and later variable declarations like
/// structs)
class Module {
 public:
  Function* create_function(std::string name, const Type* ret_type) {
    auto f = std::make_unique<Function>(std::move(name), ret_type);
    Function* raw = f.get();
    functions.push_back(std::move(f));
    return raw;
  }

  const std::vector<std::unique_ptr<Function>>& get_functions() const {
    return functions;
  }

  void addFunction(Function* function) { functions.emplace_back(function); }

 private:
  std::vector<std::unique_ptr<Function>> functions;
};
}  // namespace ir

#endif  // MODULE_H_
