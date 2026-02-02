#ifndef CRYSTALGPU_IMPL_GLAN_IMMEDIATE_H_
#define CRYSTALGPU_IMPL_GLAN_IMMEDIATE_H_

#include <CrystalBase/concepts.h>

#include "code_gen/immediate.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

template<ANY_TYPE T>
class IMMEDIATE : public code_gen::IMMEDIATE {
 public:
  using TYPE = T; // semantic static checking

  IMMEDIATE(T::CPP_TYPE VAL) :
      code_gen::IMMEDIATE(TO_STRING(VAL), T::CODE_GEN_TYPE) {
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
  }
  IMMEDIATE(const IMMEDIATE&) = delete;
  IMMEDIATE(IMMEDIATE&&) = delete;
  IMMEDIATE& operator=(const IMMEDIATE&) = delete;
  IMMEDIATE& operator=(IMMEDIATE&&) = delete;
  /* Usage */
  operator EXPRESSION<T>() const {
    PROCEDURE::PUSH(this->USAGE_);
    return EXPRESSION<T>(true);
  }
};

} // namespace crystal::gpu::impl::glan

#endif