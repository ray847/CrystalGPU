#include "CrystalGPU/device.h"

#include <webgpu/webgpu-raii.hpp>

namespace crystal::gpu {
/* Impl definition for device. */
class Device::Impl {
 public:
  Impl() {
    wgpu::raii::Instance();
  }
};

} // namespace crystal::gpu