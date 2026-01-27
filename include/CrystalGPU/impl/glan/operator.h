#ifndef CRSYTALGPU_IMPL_GLAN_OPERATOR_H_
#define CRSYTALGPU_IMPL_GLAN_OPERATOR_H_

#include <format>
#include <string>

#include "expression.h"
#include "semantic/expression.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format;

template <semantic::ANY_EXPR OPERAND>
EXPR<typename OPERAND::TYPE> operator+(const OPERAND& OP) {
  return EXPR<typename OPERAND::TYPE>(
      format("(+{})", static_cast<string>(OP)));
}

template <semantic::ANY_EXPR OPERAND>
EXPR<typename OPERAND::TYPE> operator-(const OPERAND& OP) {
  return EXPR<typename OPERAND::TYPE>(
      format("(-{})", static_cast<string>(OP)));
}

template <semantic::ANY_EXPR OPERAND1, semantic::ANY_EXPR OPERAND2>
EXPR<typename OPERAND1::TYPE> operator+(const OPERAND1& OP1,
                                        const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>,
      "Operands must be of the same type.");
  return EXPR<typename OPERAND1::TYPE>(
      format("({} + {})", static_cast<string>(OP1), static_cast<string>(OP2)));
}

template <semantic::ANY_EXPR OPERAND1, semantic::ANY_EXPR OPERAND2>
EXPR<typename OPERAND1::TYPE> operator-(const OPERAND1& OP1,
                                        const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>,
      "Operands must be of the same type.");
  return EXPR<typename OPERAND1::TYPE>(
      format("({} - {})", static_cast<string>(OP1), static_cast<string>(OP2)));
}

template <semantic::ANY_EXPR OPERAND1, semantic::ANY_EXPR OPERAND2>
EXPR<typename OPERAND1::TYPE> operator*(const OPERAND1& OP1,
                                        const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>,
      "Operands must be of the same type.");
  return EXPR<typename OPERAND1::TYPE>(
      format("({} * {})", static_cast<string>(OP1), static_cast<string>(OP2)));
}

template <semantic::ANY_EXPR OPERAND1, semantic::ANY_EXPR OPERAND2>
EXPR<typename OPERAND1::TYPE> operator/(const OPERAND1& OP1,
                                        const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>,
      "Operands must be of the same type.");
  return EXPR<typename OPERAND1::TYPE>(
      format("({} / {})", static_cast<string>(OP1), static_cast<string>(OP2)));
}

} // namespace crystal::gpu::impl::glan

#endif