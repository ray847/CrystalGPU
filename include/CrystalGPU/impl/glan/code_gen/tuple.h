#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_TUPLE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_TUPLE_H_

#include <string>
#include <vector>
#include <initializer_list>
#include <format>
#include <regex>
#include <optional>

#include "type.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::vector, std::string, std::initializer_list, std::format;

/**
 * A tuple is a data type that can hold multiple other different types.
 *
 * Objects of this class **MAYBE sliced** when copied to a regular `TYPE`
 * object. This behaviour is **intentional** and shouldn't break anything.
 */
class TUPLE : public TYPE {
 public:
  TUPLE(initializer_list<TYPE> TS) :
      TYPE(format("TUPLE_{}", DEFINITION_MAP_.size())) {
    DEFINITION_MAP_.emplace_back(TS);
  }
  inline static string DEFINITION_STATEMENT(size_t TAG) {
    string DEF = format("struct TUPLE_{} {{\n", TAG);
    for (size_t i = 0; i < DEFINITION_MAP_[TAG].size(); ++i) {
      DEF += format("\tITEM{}: {},\n", i, DEFINITION_MAP_[TAG][i].KEYWORD());
    }
    DEF += "}\n";
    return DEF;
  }

 private:
  inline static vector<vector<TYPE>> DEFINITION_MAP_;
};

using std::regex, std::smatch, std::optional;

inline optional<size_t> IS_TUPLE(const TYPE& T) {
  regex pattern{"^TUPLE_(\\d+)$"};
  smatch matches;
  if (std::regex_search(T.KEYWORD(), matches, pattern)){
    return std::stoul(matches[1].str());
  }
  return {};
}

} // namespace crystal::gpu::impl::glan::code_gen

#endif