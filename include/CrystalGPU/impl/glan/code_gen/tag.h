#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_TAG_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_TAG_H_

#include <cstdint>

namespace crystal::gpu::impl::glan::code_gen {

template<typename T>
class TAG {
 public:
  uint64_t GEN() { return NXT_++; }

 private:
  inline static uint64_t NXT_ = 0;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif