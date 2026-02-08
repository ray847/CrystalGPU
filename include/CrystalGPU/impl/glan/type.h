#ifndef CRYSTALGPU_IMPL_GLAN_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_TYPE_H_

#include <CrystalBase/fixed_string.h>

#include <string>
#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::is_same_v, std::decay_t, std::string;

template <typename T>
concept AnyType =
    requires {
      T::kKeyword;
      T::kLVal;
      T::kRVal;
      T::kCompTimeConst;
    } && is_fixed_string_v<decay_t<decltype(T::kKeyword)>>
    && is_same_v<decay_t<decltype(T::kLVal)>, bool>
    && is_same_v<decay_t<decltype(T::kRVal)>, bool>
    && is_same_v<decay_t<decltype(T::kCompTimeConst)>, bool>;

template <fixed_string keyword = "",
          bool l_val = false,
          bool r_val = false,
          bool comp_time_const = false>
struct Type {
  static constexpr fixed_string kKeyword = keyword;
  static constexpr bool kLVal = l_val;
  static constexpr bool kRVal = r_val;
  static constexpr bool kCompTimeConst = comp_time_const;
};
static_assert(AnyType<Type<>>);

struct TypeMetaData {
  string keyword;
  template <AnyType Type>
  TypeMetaData(Type) : keyword(Type::kKeyword) {
  }
};

}  // namespace crystal::gpu::impl::glan

#endif