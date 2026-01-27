#ifndef CRYSTALGPU_IMPL_GLAN_BLOCK_H_
#define CRYSTALGPU_IMPL_GLAN_BLOCK_H_

#include "code_gen/procedure.h"

namespace crystal::gpu::impl::glan {

struct BEGIN_BLOCK_RETURN {};

inline BEGIN_BLOCK_RETURN BEGIN() {
  code_gen::BEGIN_BLOCK();
  return {};
}
inline void END() {
  code_gen::END_BLOCK();
}
} // namespace crystal::gpu::impl::glan

#endif