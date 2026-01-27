#ifndef CRYSTALGPU_IMPL_GLAN_SEMANTIC_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_SEMANTIC_TYPE_H_

#include <cstdint>

namespace crystal::gpu::impl::glan::semantic {

template <typename T>
concept ANY_TYPE = requires {
  typename T::CPP_TYPE;
};

struct TYPE {
  struct I32 {
    using CPP_TYPE = int32_t;
  };
  static_assert(ANY_TYPE<TYPE::I32>);
  struct U32 {
    using CPP_TYPE = uint32_t;
  };
  static_assert(ANY_TYPE<TYPE::I32>);
};

} // namespace crystal::gpu::impl::glan::semantic

#endif