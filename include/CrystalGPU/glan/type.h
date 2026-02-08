#ifndef CRYSTALGPU_GLAN_TYPE_H_
#define CRYSTALGPU_GLAN_TYPE_H_

#include "CrystalGPU/impl/glan/dtype.h"
#include "CrystalGPU/impl/glan/proxy.h"
#include "CrystalGPU/impl/glan/storage.h"
#include "storage.h"

namespace crystal::gpu::glan {

using VarType = impl::glan::Type<"var", true, true, false>;
template <impl::glan::AnyDType DType, impl::glan::AnyStorage Storage = Local>
using Var = impl::glan::Proxy<VarType, DType, Storage>;

using ROType = impl::glan::Type<"let", false, true, false>;
template <impl::glan::AnyDType DType, impl::glan::AnyStorage Storage = Local>
using RO = impl::glan::Proxy<ROType, DType, Storage>;

using ConstType = impl::glan::Type<"const", false, true, true>;
// All consts are local.
template <impl::glan::AnyDType DType>
using Const = impl::glan::Proxy<ConstType, DType, Local>;

} // namespace crystal::gpu::glan

#endif