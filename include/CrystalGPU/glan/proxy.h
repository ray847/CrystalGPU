#ifndef CRYSTALGPU_GLAN_PROXY_H_
#define CRYSTALGPU_GLAN_PROXY_H_

#include "CrystalGPU/impl/glan/storage.h"
#include "type.h"
#include "storage.h"

namespace crystal::gpu::glan {

template<ANY_TYPE TYPE, STORAGE ST = STORAGE::LOCAL>
class PROXY {
  PROXY() {
    
  }
};

} // namespace crystal::gpu::glan

#endif