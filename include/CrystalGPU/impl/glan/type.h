#ifndef CRYSTALGPU_IMPL_GLAN_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_TYPE_H_

#include <cstdint>

#include <array>

#include <concepts>
#include <stdfloat>
#include <string>


namespace crystal::gpu::impl::glan {

template <typename T>
concept ANY_TYPE = requires {
  typename T::CPP_TYPE;
  { T::KEYWORD } -> std::same_as<const std::string&>;
};

struct TYPE {
  /* Primitive Types */
  struct I32 {
    inline static const std::string KEYWORD = "i32";
    using CPP_TYPE = int32_t;
  };
  static_assert(ANY_TYPE<TYPE::I32>);
  struct U32 {
    inline static const std::string KEYWORD = "u32";
    using CPP_TYPE = uint32_t;
  };
  static_assert(ANY_TYPE<TYPE::U32>);
  struct F32 {
    inline static const std::string KEYWORD = "f32";
    using CPP_TYPE = float;
  };
  static_assert(ANY_TYPE<TYPE::F32>);
  struct BOOL {
    inline static const std::string KEYWORD = "bool";
    using CPP_TYPE = bool;
  };
  static_assert(ANY_TYPE<TYPE::BOOL>);
  struct F16 {
    inline static const std::string KEYWORD = "f16";
#ifdef __STDCPP_FLOAT16_T__
    using CPP_TYPE = std::float16_t;
#else
    using CPP_TYPE = float;
#endif
  };
  static_assert(ANY_TYPE<TYPE::BOOL>);
  struct VOID {
    inline static const std::string KEYWORD = "void";
    using CPP_TYPE= void;
  };
  static_assert(ANY_TYPE<VOID>);

  /* Vectors */
  template <size_t N, ANY_TYPE PRIMITIVE>
  requires(N == 2) || (N == 3) || (N == 4)
  struct VEC {
    inline static const std::string KEYWORD =
        format("vec{}<{}>", N, PRIMITIVE::KEYWORD);
    using CPP_TYPE = std::array<typename PRIMITIVE::CPP_TYPE, N>;
  };
  static_assert(ANY_TYPE<VEC<2, I32>>);

  /* Matricies */
  template <size_t M, size_t N, ANY_TYPE PRIMITIVE>
  requires(M == 2) || (M == 3) || (M == 4)
  struct MAT {
    inline static const std::string KEYWORD =
      format("mat{}x{}<{}>", M, N, PRIMITIVE::KEYWORD);
    using CPP_TYPE = std::array<std::array<typename PRIMITIVE::CPP_TYPE, M>, N>;
  };
  static_assert(ANY_TYPE<MAT<2, 3, I32>>);
};
} // namespace crystal::gpu::impl::glan

#endif