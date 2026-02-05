#ifndef CRYSTALGPU_IMPL_GLAN_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_TYPE_H_

#include <CrystalBase/concepts.h>

#include <array>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::is_same_v, std::decay_t, std::is_scalar_v, std::string,
    std::string_view, std::array, std::format;

/* Concepts */
template <typename T>
concept AnyType = requires {
  typename T::cpp_type;
  { T::wgsl_keyword };
} && is_same_v<decay_t<decltype(T::wgsl_keyword)>, string_view>;

template <typename T>
concept AnyScalar = AnyType<T> && is_scalar_v<typename T::cpp_type>;

template <typename T>
concept AnyVector =
    requires { typename T::DType; } && AnyScalar<typename T::DType>
    && is_same_v<decay_t<decltype(T::N)>, size_t> && T::N >= 2 && T::N <= 4
    && is_std_array_v<typename T::cpp_type>
    && is_scalar_v<typename T::cpp_type::value_type> && AnyType<T>;

template <typename T>
concept AnyMatrix =
    requires { typename T::DType; } && AnyScalar<typename T::DType>
    && is_same_v<decay_t<decltype(T::M)>, size_t> && T::M >= 2 && T::M <= 4
    && is_same_v<decay_t<decltype(T::N)>, size_t> && T::N >= 2 && T::N <= 4
    && is_std_array_v<typename T::cpp_type>
    && is_std_array_v<typename T::cpp_type::value_type>
    && is_scalar_v<typename T::cpp_type::value_type::value_type> && AnyType<T>;

template <typename T>
concept ANY_STRUCT = AnyType<T>;

/* Scalar Type */
struct I32 {
  using cpp_type = int32_t;
  inline static constexpr string_view wgsl_keyword{ "i32" };
};
static_assert(AnyScalar<I32>);

/* Vector Type */
template <size_t dim_n, AnyScalar Scalar>
struct Vec {
  using cpp_type = array<typename Scalar::cpp_type, dim_n>;
  inline static constexpr string_view wgsl_keyword =
      format("vec{}<{}>", dim_n, Scalar::wgsl_keyword);
  using DType = Scalar;
  inline static constexpr size_t N = dim_n;
};
static_assert(AnyVector<Vec<2, I32>>);

/* Matrix Type */
template <size_t dim_m, size_t dim_n, AnyScalar Scalar>
struct Mat {
  using cpp_type = array<array<typename Scalar::cpp_type, dim_n>, dim_m>;
  inline static constexpr string_view wgsl_keyword =
      format("mat{}x{}<{}>", dim_m, dim_n, Scalar::wgsl_keyword);
  using DType = Scalar;
  inline static constexpr size_t M = dim_m, N = dim_n;
};
static_assert(AnyMatrix<Mat<2, 3, I32>>);

/* Other Types */
struct VOID {
  using cpp_type = void;
  inline static constexpr string_view wgsl_keyword{ "void" };
};
static_assert(AnyType<VOID>);

/* Type Metadata */
struct Type {
  string wgsl_keyword;
  template <AnyType T>
  Type(T) : wgsl_keyword(T::wgsl_keyword) {
  }
};

} // namespace crystal::gpu::impl::glan

#endif