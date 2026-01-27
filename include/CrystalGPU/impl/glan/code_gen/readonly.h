#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_READONLY_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_READONLY_H_

#include <format>
#include <string>

#include "expression.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;
using std::string;

class READONLY : public EXPR {
 public:
  READONLY(string KEYWORD)
      : NAME_(format("READONLY_{}_{}", TAG_.GEN(), KEYWORD)) {}
  ~READONLY() = default;
  const string& NAME() { return NAME_; }
  operator std::string() const override { return NAME_; }

 private:
  static TAG<READONLY> TAG_;

  string NAME_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif