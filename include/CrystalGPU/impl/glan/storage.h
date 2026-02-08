#ifndef CRYSTALGPU_IMPL_GLAN_STORAGE_H_
#define CRYSTALGPU_IMPL_GLAN_STORAGE_H_

#include <string>
#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::is_same_v, std::decay_t, std::string;

enum class DefLoc {
  None = 0,
  Local,
  Global,
  FnParam,
};

template <typename T>
concept AnyStorage = requires { T::kDefLoc; }
                  && is_same_v<decay_t<decltype(T::kDefLoc)>, DefLoc>;

template <DefLoc def_loc = DefLoc::None>
struct Storage {
  static constexpr DefLoc kDefLoc = def_loc;
};
static_assert(AnyStorage<Storage<>>);

struct StorageMetaData {
  DefLoc def_loc;
  template <AnyStorage Storage>
  StorageMetaData(Storage) : def_loc(Storage::kDefLoc) {
  }
};

} // namespace crystal::gpu::impl::glan

#endif