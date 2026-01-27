#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_ARGUMENT_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_ARGUMENT_H_

#include <format>
#include <list>
#include <string>
#include <algorithm>

#include "expression.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;
using std::string;

class ARG : public EXPR {
 public:
  static void CLEAR() {ARG_LIST_.clear();}
  static string LIST() {
    string CODE;
    CODE += "struct ARG_T {\n";
    for (auto& ARG : ARG_LIST_) {
      CODE += format("\t{}: {},\n", ARG.NAME(), ARG.KEYWORD());
    }
    CODE += "};\n";
    CODE += "@group(0) @binding(0) var<storage, read_write> ARG: ARG_T;\n";
    return CODE;
  }

  ARG(string KEYWORD) :
      NAME_([&] {
        string NAME = format("VAR_{}_{}", TAG_.GEN(), KEYWORD);
        std::replace(NAME.begin(), NAME.end(), '<', '_');
        std::replace(NAME.begin(), NAME.end(), '>', '_');
        return NAME;
      }()),
      KEYWORD_(KEYWORD) {
    ARG_LIST_.push_back(*this);
  }
  ~ARG() = default;
  const string& NAME() const { return NAME_; }
  const string& KEYWORD() const { return KEYWORD_; }
  operator string() const override { return format("ARG.{}", NAME_); }

 private:
  inline static TAG<ARG> TAG_{};
  inline static std::list<ARG> ARG_LIST_;

  string NAME_, KEYWORD_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif