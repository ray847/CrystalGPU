#ifndef CRYSTALGPU_IMPL_GLAN_IMMEDIATE_H_
#define CRYSTALGPU_IMPL_GLAN_IMMEDIATE_H_

#include <string>
#include <format>
#include <type_traits>

#include <CrystalBase/concepts.h>

#include "code_gen/immediate.h"
#include "semantic/immediate.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

template<ANY_TYPE T>
class IMMEDIATE : public code_gen::IMMEDIATE {
 public:
  using TYPE = T;

  IMMEDIATE(T::CPP_TYPE VAL) :
      code_gen::IMMEDIATE([=] -> std::string {
        if constexpr (std::is_integral_v<typename T::CPP_TYPE>
                      || std::is_floating_point_v<typename T::CPP_TYPE>) {
          return std::to_string(VAL);
        } else if constexpr (is_std_array<typename T::CPP_TYPE>::value
                             && !is_std_array<
                                 typename T::CPP_TYPE::value_type
                             >::value) {
          std::string IMMEDIATE_REPRESENTATION = std::format("{}(", T::KEYWORD);
          for (size_t i = 0; i < VAL.size(); ++i)
            IMMEDIATE_REPRESENTATION += std::format("{}, ", VAL[i]);
          IMMEDIATE_REPRESENTATION += ')';
          return IMMEDIATE_REPRESENTATION;
        } else if constexpr (is_std_array<typename T::CPP_TYPE>::value
                             && is_std_array<typename T::CPP_TYPE::value_type>::
                                 value) {
          std::string IMMEDIATE_REPRESENTATION = std::format("{}(", T::KEYWORD);
          for (size_t i = 0; i < VAL.size(); ++i)
            for (size_t j = 0; j < VAL[0].size(); ++j)
              IMMEDIATE_REPRESENTATION += std::format("{}, ", VAL[i]);
          IMMEDIATE_REPRESENTATION += ')';
          return IMMEDIATE_REPRESENTATION;
        } else {
          static_assert(false, "Unrecongnized Type");
        }
      }()) {
  }
};
static_assert(semantic::ANY_IMMEDIATE<IMMEDIATE<TYPE::I32>>);

} // namespace crystal::gpu::impl::glan

#endif