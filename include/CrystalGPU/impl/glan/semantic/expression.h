#ifndef CRYSTALGPU_IMPL_GLAN_SEMANTIC_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_SEMANTIC_EXPRESSION_H_

#include "type.h"

namespace crystal::gpu::impl::glan::semantic {

template <typename T>
concept ANY_EXPRESSION = requires (){
  typename T::TYPE;
} && ANY_TYPE<typename T::TYPE>;

} // namespace crystal::gpu::impl::glan::semantic

#endif