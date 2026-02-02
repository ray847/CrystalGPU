#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_TAG_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_TAG_H_

#include <cstdint>

namespace crystal::gpu::impl::glan::code_gen {

/**
 * Tag generator that generates tags in the form of 64 bit unsigned integters.
 *
 * The tags generated increments each time starting from 0. E.g., 0, 1, 2, ...
 */
class TAG {
 public:
  uint64_t GEN() {
    return NXT_++;
  }

 private:
  uint64_t NXT_ = 0;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif