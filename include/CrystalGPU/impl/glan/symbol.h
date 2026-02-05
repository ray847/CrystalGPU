#ifndef CRYSTALGPU_IMPL_GLAN_SYMBOL_H_
#define CRYSTALGPU_IMPL_GLAN_SYMBOL_H_

#include <string>

#include "type.h"
#include "storage.h"
#include "policy.h"

namespace crystal::gpu::impl::glan {

using std::string;

struct Symbol {
  string name;
  Type type;
  Storage storage;
  Behavior behavior;
};

} // namespace crystal::gpu::impl::glan

#endif