#ifndef CRYSTALGPU_IMPL_GLAN_SWIZZLE_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_SWIZZLE_TYPE_H_

#include <array>

#include "dtype.h"

namespace crystal::gpu::impl::glan {

using std::array;

/**
 * A swizzle data type.
 *
 * This data type is an intermediate type created by envoking a swizzle syntax.
 * This data type is not meant to be used by the user, but only created with
 * swizzles. Currently swizzles are not treated as containers of the inner
 * types.
 */
template <size_t n, AnyDType Inner>
using Swizzle = DType<array<typename Inner::kCppType, n>, // CppType
                      "", // keyword
                      Container<false>, // container
                      Subscript::None>; // subscripting

} // namespace crystal::gpu::impl::glan

#endif