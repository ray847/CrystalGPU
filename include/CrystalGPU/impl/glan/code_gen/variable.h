#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_VARIABLE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_VARIABLE_H_

#include <format>
#include <string>
#include <algorithm>

#include "expression.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;
using std::string;

class VAR : public EXPR {
 public:
  VAR(string KEYWORD) :
      NAME_([&] {
        string NAME = format("VAR_{}_{}", TAG_.GEN(), KEYWORD);
        std::replace(NAME.begin(), NAME.end(), '<', '_');
        std::replace(NAME.begin(), NAME.end(), '>', '_');
        return NAME;
      }()),
      KEYWORD_(KEYWORD) {
  }
  VAR(string KEYWORD, string NAME)
      : NAME_(NAME), KEYWORD_(KEYWORD) {}
  virtual ~VAR() = default;
  const string& NAME() const { return NAME_; }
  const string& KEYWORD() const { return KEYWORD_; }
  operator std::string() const override { return NAME_; };

 private:
  inline static TAG<VAR> TAG_{};

  string NAME_;
  string KEYWORD_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif