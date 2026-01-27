#ifndef CRYSTALGPU_IMPL_GLAN_READONLY_H_
#define CRYSTALGPU_IMPL_GLAN_READONLY_H_

#include "code_gen/readonly.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

template <ANY_TYPE T>
class READONLY : public code_gen::READONLY {
 public:
  using TYPE = T;

	READONLY() : code_gen::READONLY(T::KEYWORD) {}
	~READONLY() = default;
};

} // namespace crystal::gpu::impl::glan

#endif