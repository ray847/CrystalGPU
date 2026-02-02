#ifndef CRYSTALGPU_IMPL_GLAN_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_TYPE_H_

#include <CrystalBase/concepts.h>

#include <array>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <stdfloat>
#include <string>
#include <type_traits>
#include <tuple>
#include <vector>

#include "code_gen/type.h"
#include "code_gen/tuple.h"
#include "semantic/type.h"

namespace crystal::gpu::impl::glan {

using std::format, std::array, std::string, std::tuple, std::vector;

/* Type Concept */
template <typename TYPE>
concept ANY_TYPE = requires() {
  { TYPE::CODE_GEN_TYPE } -> std::same_as<const code_gen::TYPE&>;
} && semantic::ANY_TYPE<TYPE>;

/* Primitives */
class INT32 {
 public:
  using CPP_TYPE = int32_t;
  inline const static code_gen::TYPE CODE_GEN_TYPE{ "i32" };
};
static_assert(ANY_TYPE<INT32>);

/* Vector */
template <size_t n, ANY_TYPE T>
class VECTOR {
 public:
  using CPP_TYPE = array<typename T::CPP_TYPE, n>;
  inline const static code_gen::TYPE CODE_GEN_TYPE{ format(
      "vec{}<{}>", n, T::CODE_GEN_TYPE.KEYWORD()) };
};
static_assert(ANY_TYPE<VECTOR<2, INT32>>);

/* Matrix */
template <size_t m, size_t n, ANY_TYPE T>
class MATRIX {
 public:
  using CPP_TYPE = array<array<typename T::CPP_TYPE, n>, m>;
  inline const static code_gen::TYPE CODE_GEN_TYPE{ format(
      "mat{}x{}<{}>", m, n, T::CODE_GEN_TYPE.KEYWORD()) };
};
static_assert(ANY_TYPE<MATRIX<2, 3, INT32>>);

/* Tuple */
template <typename... TYPES>
class TUPLE : public code_gen::TUPLE {
 public:
  using CPP_TYPE = tuple<TYPES...>;
  inline const static code_gen::TYPE CODE_GEN_TYPE = code_gen::TUPLE{
    std::initializer_list<code_gen::TYPE>{ (TYPES::CODE_GEN_TYPE)... }
  };
};
static_assert(ANY_TYPE<TUPLE<INT32>>);
template <typename TYPE>
struct ANY_TUPLE : public std::false_type {};
template <typename... TYPES>
struct ANY_TUPLE<TUPLE<TYPES...>> : public std::true_type {};

template <typename CPP_TYPE>
string TO_STRING(CPP_TYPE VAL) {
  if constexpr (std::is_integral_v<CPP_TYPE>
                || std::is_floating_point_v<CPP_TYPE>) {
    return std::to_string(VAL);
  } else if constexpr (is_std_array<CPP_TYPE>::value
                       && (std::is_integral_v<typename CPP_TYPE::value_type>
                           || std::is_floating_point_v<
                               typename CPP_TYPE::value_type
                           >)) {
    string STR = "{";
    for (auto V : VAL) STR += format("{}, ", V);
    STR += "}";
    return STR;
  } else if constexpr (is_std_array<CPP_TYPE>::value
                       && is_std_array<typename CPP_TYPE::value_type>::value
                       && (std::is_integral_v<
                               typename CPP_TYPE::value_type::value_type
                           >
                           || std::is_floating_point_v<
                               typename CPP_TYPE::value_type::value_type
                           >)) {
    string STR = "{";
    for (auto& VA : VAL)
      for (auto V : VA) STR += format("{}, ", V);
    STR += "}";
    return STR;
  }
}

} // namespace crystal::gpu::impl::glan

#endif