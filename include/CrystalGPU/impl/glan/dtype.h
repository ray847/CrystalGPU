#ifndef CRYSTALGPU_IMPL_GLAN_DTYPE_H_
#define CRYSTALGPU_IMPL_GLAN_DTYPE_H_

#include <CrystalBase/fixed_string.h>

#include <array>
#include <cstddef>
#include <format>
#include <string>
#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::array, std::format, std::is_same_v, std::decay_t, std::string;

/* Subscript */
enum class Subscript {
  None = 0,
  True,
  Swizzle,
};

/* Container */
template <bool is_container = false, size_t n = 0, typename Inner = void>
struct Container {
  static constexpr bool kIsContainer = false;
  static constexpr size_t kN = 0;
  using kInner = Inner;
  operator bool() const {
    return is_container;
  }
};
template <typename T>
struct AnyContainerHelper : std::false_type {};
template <bool is_container, size_t n, typename Inner>
struct AnyContainerHelper<Container<is_container, n, Inner>> : std::true_type {
};
template <typename T>
concept AnyContainer = AnyContainerHelper<T>::value;

/* DType Concept */
template <typename T>
concept AnyDType = requires {
  typename T::kCppType;
  typename T::kContainer;
  T::kSubscript;
} && is_fixed_string_v<decay_t<decltype(T::kKeyword)>>
&& AnyContainer<typename T::kContainer>
&& is_same_v<decay_t<decltype(T::kSubscript)>, Subscript>;

/* DType Template */
template <typename CppType = void,
          fixed_string keyword = "",
          AnyContainer Container = Container<>,
          Subscript subscript = Subscript::None>
struct DType {
  using kCppType = CppType;
  static constexpr fixed_string kKeyword = keyword;
  using kContainer = Container;
  static constexpr Subscript kSubscript = subscript;
};
static_assert(AnyDType<DType<>>);

/* Metadata */
struct DTypeMetaData {
  string keyword;
  DTypeMetaData(string keyword) : keyword(keyword) {
  }
  template <AnyDType DType>
  DTypeMetaData(DType) : keyword(DType::kKeyword) {
  }
};

} // namespace crystal::gpu::impl::glan

#endif