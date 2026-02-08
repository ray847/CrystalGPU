#ifndef CRYSTALGPU_GLAN_DTYPE_H_
#define CRYSTALGPU_GLAN_DTYPE_H_

#include <cstdint>

#include <CrystalBase/fixed_string.h>

#include "CrystalGPU/impl/glan/dtype.h"

namespace crystal::gpu::glan {

template <typename CppType, fixed_string keyword>
using Scalar = impl::glan::DType<CppType,
                                 keyword,
                                 impl::glan::Container<false>,
                                 impl::glan::Subscript::None>;

using I32 = Scalar<int32_t, "i32">;
using U32 = Scalar<uint32_t, "u32">;
using F32 = Scalar<float, "f32">;
using Bool = Scalar<bool, "bool">;

} // namespace crystal::gpu::glan

#endif