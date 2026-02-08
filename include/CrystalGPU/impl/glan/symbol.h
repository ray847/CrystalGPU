#ifndef CRYSTALGPU_IMPL_GLAN_SYMBOL_H_
#define CRYSTALGPU_IMPL_GLAN_SYMBOL_H_

#include <string>

#include "dtype.h"
#include "storage.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string;

struct Symbol {
  string name;
  TypeMetaData type_metadata;
  DTypeMetaData dtype_metadata;
  StorageMetaData storage_metadata;
};

} // namespace crystal::gpu::impl::glan

#endif