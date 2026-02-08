#ifndef CRYSTALGPU_GLAN_STORAGE_H_
#define CRYSTALGPU_GLAN_STORAGE_H_

#include "CrystalGPU/impl/glan/storage.h"

namespace crystal::gpu::glan {

using Local = impl::glan::Storage<impl::glan::DefLoc::Local>;

using Storage = impl::glan::Storage<impl::glan::DefLoc::Global>;

using Param = impl::glan::Storage<impl::glan::DefLoc::FnParam>;

} // namespace crystal::gpu::glan

#endif