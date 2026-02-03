#ifndef CRSYTALGPU_IMPL_GLAN_OPERATOR_H_
#define CRSYTALGPU_IMPL_GLAN_OPERATOR_H_

#include <format>
#include <string>

#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format;

template <ANY_EXPRESSION OPERAND>
EXPRESSION<typename OPERAND::TYPE> operator+(const OPERAND& OP) {
  /* This pushes OP1 to the procedure. */
  auto OP_EXPR = static_cast<EXPRESSION<typename OPERAND::TYPE>>(OP);
  /* Pop the previous string and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("(+{})", STACK.POP()));
  });
  return OP_EXPR;
}

template <ANY_EXPRESSION OPERAND>
EXPRESSION<typename OPERAND::TYPE> operator-(const OPERAND& OP) {
  /* This pushes OP1 to the procedure. */
  auto OP_EXPR = static_cast<EXPRESSION<typename OPERAND::TYPE>>(OP);
  /* Pop the previous string and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("(-{})", STACK.POP()));
  });
  return OP_EXPR;
}

template <ANY_EXPRESSION OPERAND1, ANY_EXPRESSION OPERAND2>
EXPRESSION<typename OPERAND1::TYPE> operator+(const OPERAND1& OP1,
                                              const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>);
  /* This pushes OP1 to the procedure. */
  auto OP1_EXPR = static_cast<EXPRESSION<typename OPERAND1::TYPE>>(OP1);
  /* This pushes OP2 to the procedure. */
  auto OP2_EXPR = static_cast<EXPRESSION<typename OPERAND2::TYPE>>(OP2);
  /* Pop the previous 2 strings and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("({} + {})", STACK.POP(), STACK.POP()));
  });
  return OP1_EXPR && OP2_EXPR;
}

template <ANY_EXPRESSION OPERAND1, ANY_EXPRESSION OPERAND2>
EXPRESSION<typename OPERAND1::TYPE> operator-(const OPERAND1& OP1,
                                              const OPERAND2& OP2) {
  static_assert(
      std::is_same_v<typename OPERAND1::TYPE, typename OPERAND2::TYPE>);
  /* This pushes OP1 to the procedure. */
  auto OP1_EXPR = static_cast<EXPRESSION<typename OPERAND1::TYPE>>(OP1);
  /* This pushes OP2 to the procedure. */
  auto OP2_EXPR = static_cast<EXPRESSION<typename OPERAND2::TYPE>>(OP2);
  /* Pop the previous 2 strings and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("({} - {})", STACK.POP(), STACK.POP()));
  });
  return OP1_EXPR && OP2_EXPR;
}

template <ANY_EXPRESSION OPERAND1, ANY_EXPRESSION OPERAND2>
EXPRESSION<typename OPERAND1::TYPE> operator*(const OPERAND1& OP1,
                                              const OPERAND2& OP2) {
  static_assert(ANY_SCALAR<typename OPERAND1::TYPE>
                || ANY_SCALAR<typename OPERAND2::TYPE>);
  /* This pushes OP1 to the procedure. */
  auto OP1_EXPR = static_cast<EXPRESSION<typename OPERAND1::TYPE>>(OP1);
  /* This pushes OP2 to the procedure. */
  auto OP2_EXPR = static_cast<EXPRESSION<typename OPERAND2::TYPE>>(OP2);
  /* Pop the previous 2 strings and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("({} * {})", STACK.POP(), STACK.POP()));
  });
  return OP1_EXPR && OP2_EXPR;
}

template <ANY_EXPRESSION OPERAND1, ANY_EXPRESSION OPERAND2>
EXPRESSION<typename OPERAND1::TYPE> operator/(const OPERAND1& OP1,
                                              const OPERAND2& OP2) {
  static_assert(ANY_SCALAR<typename OPERAND2::TYPE>);
  /* This pushes OP1 to the procedure. */
  auto OP1_EXPR = static_cast<EXPRESSION<typename OPERAND1::TYPE>>(OP1);
  /* This pushes OP2 to the procedure. */
  auto OP2_EXPR = static_cast<EXPRESSION<typename OPERAND2::TYPE>>(OP2);
  /* Pop the previous 2 strings and push the expression. */
  PROCEDURE::PUSH([](auto STACK) {
    STACK.PUSH(format("({} / {})", STACK.POP(), STACK.POP()));
  });
  return OP1_EXPR && OP2_EXPR;
}

} // namespace crystal::gpu::impl::glan

#endif