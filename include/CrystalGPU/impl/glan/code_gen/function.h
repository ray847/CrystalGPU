#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_FUNCTION_H_

#include <format>
#include <list>
#include <string>

#include "block.h"
#include "tag.h"
#include "argument.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;
using std::string;

class SIGNATURE {
 public:
  string NAME_;
  string RETURN_KEYWORD_;

  SIGNATURE(string RETURN_KEYWORD)
      : NAME_(format("FUNCTION_{}", TAG_.GEN())),
        RETURN_KEYWORD_(RETURN_KEYWORD) {}
  operator std::string() { return NAME_; }

 private:
  inline static TAG<SIGNATURE> TAG_{};
};

class FUNCTION {
 public:
  SIGNATURE SIGNATURE_;
  BLOCK CODE_BLOCK_{};
  std::list<ARG> PARAMS_{};

  FUNCTION(string RETURN_KEYWORD) : SIGNATURE_(RETURN_KEYWORD) {};
  FUNCTION(const FUNCTION& other) = delete;
  FUNCTION(FUNCTION&& other) = default;
  FUNCTION& operator=(const FUNCTION& rhs) = delete;
  FUNCTION& operator=(FUNCTION&& rhs) = delete;

  operator string() const {
    string RES;
    string PARAM_STR;
    for (auto& VAR : PARAMS_)
      PARAM_STR += format("{}: {}, ", VAR.NAME(), VAR.KEYWORD());
    RES += format("fn {}({}) -> {} ", SIGNATURE_.NAME_, PARAM_STR,
                  SIGNATURE_.RETURN_KEYWORD_);
    RES += CODE_BLOCK_;
    return RES;
  }
};
} // namespace crystal::gpu::impl::glan::code_gen

#endif