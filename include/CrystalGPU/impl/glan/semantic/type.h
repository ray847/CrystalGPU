#ifndef CRYSTALGPU_IMPL_GLAN_SEMANTIC_TYPE_H_
#define CRYSTALGPU_IMPL_GLAN_SEMANTIC_TYPE_H_

namespace crystal::gpu::impl::glan::semantic {

template <typename T>
concept ANY_TYPE = requires() { typename T::CPP_TYPE; };

} // namespace crsytal::gpu::impl::glan::semantic

#endif