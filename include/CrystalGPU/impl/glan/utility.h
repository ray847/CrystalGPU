#ifndef CRYSTALGPU_IMPL_GLAN_UTILITY_H_
#define CRYSTALGPU_IMPL_GLAN_UTILITY_H_

#include <cstdint>

#include <string>
#include <string_view>
#include <format>

namespace crystal::gpu::impl::glan {

using std::string, std::string_view, std::format;

inline string GenName(const string& prefix = "_", const string& suffix = "") {
  static uint64_t nxt_tag = 0;
  return format("{}_{}_{}", prefix, nxt_tag++, suffix);
}

} // namespace crystal::gpu::impl::glan

#endif