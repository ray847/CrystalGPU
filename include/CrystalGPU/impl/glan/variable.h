#ifndef CRYSTALGPU_IMPL_GLAN_VARIABLE_H_
#define CRYSTALGPU_IMPL_GLAN_VARIABLE_H_

#include <format>
#include <string>

#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format, std::string;

struct ARG {};
struct RET {};
struct STK {};
template <typename T>
concept VAR_STORAGE_TYPE =
    std::is_same_v<T, ARG> || std::is_same_v<T, RET> || std::is_same_v<T, STK>;

template <ANY_TYPE T, typename TYPE = STK>
class VAR;

} // namespace crystal::gpu::impl::glan

#endif