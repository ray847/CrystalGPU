#ifndef CRYSTALGPU_IMPL_GLAN_POLICY_H_
#define CRYSTALGPU_IMPL_GLAN_POLICY_H_

#include <CrystalBase/string_literal.h>

#include <type_traits>

namespace crystal::gpu::impl::glan {

using std::is_same_v, std::decay_t;

/**
 * Definition location.
 *
 * Options:
 *  * Local: For local variables, etc.
 *  * Global: For buffers, uniforms, etc.
 */
enum class DefLoc {
  Local,
  Global,
};

template <typename T>
concept AnyPolicy = is_same_v<decay_t<decltype(T::kDefLoc)>, DefLoc>
                 && is_string_literal_v<decay_t<decltype(T::kDefKeyword)>>
                 && is_same_v<decay_t<decltype(T::kAssignable)>, bool>
                 && is_same_v<decay_t<decltype(T::kSubscriptable)>, bool>
                 && is_same_v<decay_t<decltype(T::kSwizzle)>, bool>
                 && is_same_v<decay_t<decltype(T::kCompTimeConst)>, bool>;

/**
 * A policy for how a symbol should behave.
 *
 * This class doesn't implement behaviour. The implementation is in the `Proxy`
 * class, which takes in a `Policy` template arguement.
 */
template <DefLoc def_loc = DefLoc::Local,
          string_literal def_keyword = "empty_def_keyword",
          bool assignable = true,
          bool subscriptable = false,
          bool swizzle = false,
          bool comp_time_const = false>
struct Policy {
  inline static constexpr string_literal kDefKeyword = def_keyword;
  inline static constexpr DefLoc kDefLoc = def_loc;
  inline static constexpr bool kAssignable = assignable;
  inline static constexpr bool kSubscriptable = comp_time_const;
  inline static constexpr bool kSwizzle = swizzle;
  inline static constexpr bool kCompTimeConst = comp_time_const;
};
static_assert(AnyPolicy<Policy<>>);

struct Behavior {
  /* Constructor */
  template <AnyPolicy Policy>
  Behavior(Policy p) :
      def_loc(Policy::kDefLoc),
      def_keyword(Policy::kDefKeyword),
      assignable(Policy::kAssignable),
      subscriptable(Policy::kSubscriptable),
      swizzle(Policy::kSwizzle),
      comp_time_const(Policy::kCompTimeConst) {
  }
  DefLoc def_loc;
  string def_keyword;
  bool assignable;
  bool subscriptable;
  bool swizzle;
  bool comp_time_const;
};

};  // namespace crystal::gpu::impl::glan

#endif