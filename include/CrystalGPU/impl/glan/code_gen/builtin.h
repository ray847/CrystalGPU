#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_BUILTIN_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_BUILTIN_H_

#include <string>
#include <format>
#include <list>

#include "variable.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::string, std::format;

class BUILTIN : public VAR {
 public:
  BUILTIN(string KEYWORD, string NAME, string DECORATOR) :
      VAR(KEYWORD, NAME), DECORATOR_(DECORATOR) {
  }
  ~BUILTIN() = default;

 private:
  inline static std::list<BUILTIN> BUILTIN_LIST_;
  string DECORATOR_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif