#ifndef CRYSTALGPU_IMPL_GLAN_SEMANTIC_VARIABLE_H_
#define CRYSTALGPU_IMPL_GLAN_SEMANTIC_VARIABLE_H_

#include "expression.h"

namespace crystal::gpu::impl::glan::semantic {

template<typename T>
concept ANY_VAR = ANY_EXPR<T>;

} // namespace crystal::gpu::impl::glan::semantics

#endif