#ifndef CRYSTALGPU_IMPL_GLAN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_FUNCTION_H_

#include <format>
#include <string>
#include <vector>

#include "block.h"
#include "dtype.h"
#include "symbol.h"
#include "utility.h"

namespace crystal::gpu::impl::glan {

using std::string, std::vector, std::format;

/**
 * A signature for a `glan` function.
 *
 * A signature contains:
 *  1. name
 *  2. return type
 *  3. params
 */
class Signature {
 public:
  /* Variables */
  Signature(DTypeMetaData RET_TYPE) :
      name_(GenName("sig")), ret_type_(RET_TYPE) {
  }
  Signature(DTypeMetaData RET_TYPE, string NAME) :
      name_(NAME), ret_type_(RET_TYPE) {
  }
  string name_;
  DTypeMetaData ret_type_;
  vector<Symbol> params_;
};

class Fn {
 public:
  Fn(DTypeMetaData RET_TYPE) : sig_(RET_TYPE) {
  }
  Fn(DTypeMetaData RET_TYPE, string NAME) : sig_(RET_TYPE, NAME) {
  }
  auto& Sig() {
    return sig_;
  }
  auto& Def() {
    return def_;
  }
  operator string() const {
    string code;
    {
      string param_str;
      if (sig_.params_.size() > 0) {
        param_str += format("{}: {}",
                            sig_.params_[0].name,
                            sig_.params_[0].dtype_metadata.keyword);
        for (int i = 1; i < sig_.params_.size(); ++i)
          param_str += format(", {}: {}",
                              sig_.params_[0].name,
                              sig_.params_[0].dtype_metadata.keyword);
      }
      code += format("fn {}({}) ", sig_.name_, param_str);
    }
    if (sig_.ret_type_.keyword != "void")
      code += format("-> {} ", sig_.ret_type_.keyword);
    code += def_;
    code += '\n';
    return code;
  }

 private:
  /* Function Signature */
  Signature sig_;
  /* Function Definition */
  BLK def_;
};

} // namespace crystal::gpu::impl::glan

#endif