#ifndef CRYSTALGPU_GLAN_PROCEDURE_H_
#define CRYSTALGPU_GLAN_PROCEDURE_H_

#include "CrystalGPU/impl/glan/procedure.h"
#include "CrystalGPU/impl/glan/proxy.h"

namespace crystal::gpu::glan {

using impl::glan::BindProc, impl::glan::UnbindProc;

class Proc : public impl::glan::Proc {};

} // namespace crystal::gpu::glan

#endif