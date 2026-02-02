#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_TYPE_H_

#include <string>

namespace crystal::gpu::impl::glan::code_gen {

using std::string;

class TYPE {
 public:
  TYPE(const string& KEYWORD = {}) : KEYWORD_(KEYWORD) {}
  auto& KEYWORD() {return KEYWORD_;}
  const auto& KEYWORD() const {return KEYWORD_;}

 private:
  string KEYWORD_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif