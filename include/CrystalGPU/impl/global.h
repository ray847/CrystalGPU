#ifndef CRYSTALGPU_IMPL_GLOBAL_H_
#define CRYSTALGPU_IMPL_GLOBAL_H_

#include <expected>
#include <format>
#include <stacktrace>
#include <string>

#include <webgpu/webgpu-raii.hpp>
#include <webgpu/webgpu.hpp>

namespace crystal::gpu::impl {
class Context {
 public:
  /* Functions */
  std::expected<void, std::string> Init() {
    instance_ = wgpu::createInstance(wgpu::InstanceDescriptor{});
    if (!instance_) {
      return std::unexpected(
          std::format("CrystalGPU context init failed at:\n{}",
                      std::stacktrace::current()));
    }
    return {};
  }
  std::expected<void, std::string> Term() {
    instance_->release();
    return {};
  }
  std::expected<void, std::string> Sync() {
    instance_->processEvents();
    return {};
  }
  auto& WGPUInstance() { return instance_; }
  operator bool() { return static_cast<bool>(instance_); }

 private:
  /* Variables */
  wgpu::raii::Instance instance_;
};

inline Context context{};

} // namespace crystal::gpu::impl

#endif