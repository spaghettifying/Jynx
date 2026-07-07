#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "sourcelocation.hh"

class Diagnostics {
 public:
  static Diagnostics& instance() {
    static Diagnostics instance;
    return instance;
  }

  Diagnostics(const Diagnostics&) = delete;
  Diagnostics& operator=(const Diagnostics&) = delete;

  void clear() {
    errors.clear();
    warnings.clear();
  }

  void report_error(const std::string& error_kind, const std::string& message,
                    SourceLocation location) {
    errors[error_kind].push_back(message + " [" + location.to_string() + "]");
  }

  void report_warning(const std::string& warning_kind,
                      const std::string& message, SourceLocation location) {
    warnings[warning_kind].push_back(message + " [" + location.to_string() +
                                     "]");
  }

  bool has_errors() const { return !errors.empty(); }
  bool has_errors(const std::string& error_kind) const {
    auto it = errors.find(error_kind);
    return it != errors.end() && !it->second.empty();
  }

  size_t error_count() const { return errors.size(); }
  size_t error_count(const std::string& error_kind) const {
    auto it = errors.find(error_kind);
    return it == errors.end() ? 0 : it->second.size();
  }

  const std::unordered_map<std::string, std::vector<std::string>>& get_errors()
      const {
    return errors;
  }

  const std::unordered_map<std::string, std::vector<std::string>>&
  get_warnings() const {
    return warnings;
  }

  const std::vector<std::string>& get_errors(
      const std::string& error_kind) const {
    auto it = errors.find(error_kind);
    static const std::vector<std::string> empty;
    return it == errors.end() ? empty : it->second;
  }

  const std::vector<std::string>& get_warnings(
      const std::string& warning_kind) const {
    auto it = warnings.find(warning_kind);
    static const std::vector<std::string> empty;
    return it == warnings.end() ? empty : it->second;
  }

 private:
  Diagnostics() = default;

  std::unordered_map<std::string, std::vector<std::string>> errors;
  std::unordered_map<std::string, std::vector<std::string>> warnings;
};

#endif  // DIAGNOSTICS_H_
