#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_RETUMENT_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_RETUMENT_H_

#include <format>
#include <list>
#include <string>
#include <algorithm>

#include "expression.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;
using std::string;

class RET : public EXPR {
 public:
  static string LIST() {
    string CODE;
    CODE += "struct RET_T {\n";
    for (auto& RET : RET::RET_LIST_) {
      CODE += format("\t{}: {},\n", RET.NAME(), RET.KEYWORD());
    }
    CODE += "};\n";
    CODE += "@group(0) @binding(1) var<storage, read_write> RET: RET_T;\n";
    return CODE;
  }
  static void CLEAR() { RET::RET_LIST_.clear(); }

  RET(string KEYWORD) :
      NAME_([&] {
        string NAME = format("VAR_{}_{}", TAG_.GEN(), KEYWORD);
        std::replace(NAME.begin(), NAME.end(), '<', '_');
        std::replace(NAME.begin(), NAME.end(), '>', '_');
        return NAME;
      }()),
      KEYWORD_(KEYWORD) {
    RET_LIST_.push_back(*this);
  }
  ~RET() = default;
  const string& NAME() const { return NAME_; }
  const string& KEYWORD() const { return KEYWORD_; }
  operator string() const override {return format("RET.{}", NAME_);}

 private:
  inline static TAG<RET> TAG_{};
  inline static std::list<RET> RET_LIST_{};

  string NAME_, KEYWORD_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif