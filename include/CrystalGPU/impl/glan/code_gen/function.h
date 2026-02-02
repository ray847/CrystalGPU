#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_FUNCTION_H_

#include <vector>
#include <string>
#include <format>
#include <sstream>

#include "block.h"
#include "atomic.h"
#include "type.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::vector, std::string, std::stringstream;

class SIGNATURE {
 public:
  vector<ATOMIC> PARAMS_;
  TYPE TYPE_;
  string NAME_;
};

class FUNCTION {
 public:
  FUNCTION(SIGNATURE SIG) : SIG_(SIG) {
  }
  auto& SIGNATURE() {return SIG_;}
  const auto& SIGNATURE() const {return SIG_;}
  auto& BLOCK() {return BLK_;}
  const auto& BLOCK() const {return BLK_;}
  string DEFINITION() const {
    stringstream PARAMS;
    if (SIG_.PARAMS_.size()) {
      PARAMS << format(
          "{}: {}", SIG_.PARAMS_[0].USAGE_, SIG_.PARAMS_[0].TYPE_.KEYWORD());
      for (size_t i = 0; i < SIG_.PARAMS_.size(); ++i) {
        PARAMS << format(", {}: {}",
                         SIG_.PARAMS_[0].USAGE_,
                         SIG_.PARAMS_[0].TYPE_.KEYWORD());
      }
    }
    string RES;
    if (SIG_.TYPE_.KEYWORD() == "void") [[unlikely]]
      RES += format("fn {}({}) ", SIG_.NAME_, PARAMS.str());
    else
      RES += format(
          "fn {}({}) -> {} ", SIG_.NAME_, PARAMS.str(), SIG_.TYPE_.KEYWORD());
    RES += BLK_;
    RES += '\n';
    return RES;
  }

 private:
  class SIGNATURE SIG_;
  class BLOCK BLK_;
};
} // namespace crystal::gpu::impl::glan::code_gen

#endif