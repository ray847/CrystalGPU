#ifndef CRYSTALGPU_IMPL_GLAN_ALIAS_H_
#define CRYSTALGPU_IMPL_GLAN_ALIAS_H_

#include "semantic/expression.h"
#include "variable.h"
#include "immediate.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

/* Template deduction guide. Constructed VAR defaults to STK. */
template <semantic::ANY_EXPR E>
VAR(E) -> VAR<typename E::TYPE, STK>;

/* Variable Types */
template <VAR_STORAGE_TYPE T>
using I32 = VAR<TYPE::I32, T>;

template <VAR_STORAGE_TYPE T = STK>
using U32 = VAR<TYPE::U32, T>;

template <VAR_STORAGE_TYPE T = STK>
using F32 = VAR<TYPE::F32, T>;

template <VAR_STORAGE_TYPE T = STK>
using BOOL = VAR<TYPE::BOOL, T>;

template <VAR_STORAGE_TYPE T = STK>
using F16 = VAR<TYPE::F16, T>;

template <size_t N,
          template <VAR_STORAGE_TYPE> class PRIMITIVE_TYPE,
          VAR_STORAGE_TYPE T = STK>
requires ANY_TYPE<typename PRIMITIVE_TYPE<STK>::TYPE>
using VEC = VAR<TYPE::VEC<N, typename PRIMITIVE_TYPE<STK>::TYPE>, T>;

template <size_t M,
          size_t N,
          template <VAR_STORAGE_TYPE> class PRIMITIVE_TYPE,
          VAR_STORAGE_TYPE T = STK>
requires ANY_TYPE<typename PRIMITIVE_TYPE<STK>::TYPE>
using MAT = VAR<TYPE::MAT<M, N, typename PRIMITIVE_TYPE<STK>::TYPE>, T>;

/* Immediate Types */
inline IMMEDIATE<TYPE::I32> operator ""_I32(unsigned long long VAL) {
  return {static_cast<TYPE::I32::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::U32> operator ""_U32(unsigned long long VAL) {
  return {static_cast<TYPE::U32::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::F32> operator ""_F32(unsigned long long VAL) {
  return {static_cast<TYPE::F32::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::F32> operator ""_F32(long double VAL) {
  return {static_cast<TYPE::F32::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::BOOL> operator ""_BOOL(unsigned long long VAL) {
  return {static_cast<TYPE::BOOL::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::F16> operator ""_F16(long double VAL) {
  return {static_cast<TYPE::F16::CPP_TYPE>(VAL)};
}
inline IMMEDIATE<TYPE::F16> operator ""_F16(unsigned long long VAL) {
  return {static_cast<TYPE::F16::CPP_TYPE>(VAL)};
}

} // namespace crystal::gpu::impl::glan

#endif