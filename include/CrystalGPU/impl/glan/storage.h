#ifndef CRYSTALGPU_IMPL_GLAN_STORAGE_H_
#define CRYSTALGPU_IMPL_GLAN_STORAGE_H_

#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::is_same_v;

enum class Storage {
  UNIFORM,
  SSBO,
  LOCAL,
};

} // namespace crystal::gpu::impl::glan

#endif