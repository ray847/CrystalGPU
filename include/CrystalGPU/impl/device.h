#ifndef CRYSTALGPU_IMPL_DEVICE_H_
#define CRYSTALGPU_IMPL_DEVICE_H_

#include <cstring>

#include <iostream>
#include <span>
#include <array>
#include <expected>
#include <optional>
#include <sstream>

#include <webgpu/webgpu-raii.hpp>
#include <webgpu/webgpu.hpp>
#include <webgpu/webgpu.h>

#include "global.h"

namespace crystal::gpu::impl {

inline wgpu::Adapter CreateWGPUAdapter() {
  wgpu::RequestAdapterOptions ops{};
  ops.compatibleSurface = nullptr;
  return context.WGPUInstance()->requestAdapter(ops);
}

/**
 * Create a WebGPU device from a WebGPU adapter.
 *
 * @param adapter The WebGPU Adapter to request from.
 * @param error_destination This `optional` will be set to empty after function
 * call. If some error happens to the device, this variable will be filled with
 * some error message.
 * @return The created device.
 */
inline wgpu::Device CreateWGPUDevice(
    wgpu::Adapter& adapter, std::optional<std::string>& error_destination) {
  wgpu::Limits limits;
  adapter.getLimits(&limits);

  wgpu::DeviceDescriptor desc{};
  desc.label = wgpu::StringView("CrystalGPU Device");
  desc.requiredFeatureCount = 0;
  desc.requiredLimits = &limits;
  desc.deviceLostCallbackInfo.mode = wgpu::CallbackMode::AllowProcessEvents;
  desc.defaultQueue.label = wgpu::StringView("Default CrystalGPU Queue");
  desc.deviceLostCallbackInfo.userdata1 =
      reinterpret_cast<void*>(&error_destination);
  desc.deviceLostCallbackInfo.callback = [](const WGPUDevice* device,
                                            WGPUDeviceLostReason reason,
                                            WGPUStringView msg,
                                            void* userdata1,
                                            void*) {
    auto& error_destination =
        *static_cast<std::optional<std::string>*>(userdata1);
    if (error_destination) {
      std::cout << "Unhandled error:" << error_destination.value();
      //abort();
    }
    std::stringstream ss;
    ss << "WebGPU device lost: ";
    ss << "Reason: " << reason;
    ss << "Message: " << wgpu::StringView(msg);
    error_destination = ss.str();
  };
  desc.uncapturedErrorCallbackInfo.userdata1 =
      reinterpret_cast<void*>(&error_destination);
  desc.uncapturedErrorCallbackInfo.callback = [](const WGPUDevice* device,
                                                 WGPUErrorType reason,
                                                 WGPUStringView msg,
                                                 void* userdata1,
                                                 void*) {
    auto& error_destination =
        *static_cast<std::optional<std::string>*>(userdata1);
    if (error_destination) {
      std::cout << "Unhandled error:" << error_destination.value();
      //abort();
    }
    std::stringstream ss;
    ss << "WebGPU device lost: ";
    ss << "Reason: " << reason;
    ss << "Message: " << wgpu::StringView(msg);
    error_destination = ss.str();
  };
  wgpu::Device device = adapter.requestDevice(desc);
  context.WGPUInstance()->processEvents();
  return device;
}

/**
 * Every object of this class represents a physical GPU device.
 *
 * This class handles the following:
 *  - Running procedures (GPU code).
 *  - Caching procedures that have been run so that repeated runs are
 * accelerated.
 */
class Device {
 public:
  Device() {
    InitDevice();
    InitBindGroupLayout();
  }
  std::expected<void, std::string> Run(std::string code,
                                       std::span<const std::byte> arguments,
                                       std::span<std::byte> returns) {
    if (auto error = CheckError()) return std::unexpected(error.value());

    wgpu::raii::ShaderModule shader = CreateShader(code);

    /* Check for errors. */
    if (auto error = CheckError()) return std::unexpected(error.value());

    wgpu::raii::PipelineLayout pipeline_layout = CreatePipelineLayout();

    wgpu::raii::ComputePipeline pipeline =
        CreatePipeline(*shader, *pipeline_layout);

    /* Check for errors. */
    if (auto error = CheckError()) return std::unexpected(error.value());

    wgpu::raii::Buffer arg_buffer = CreateArgBuffer(arguments.size());
    wgpu::raii::Buffer ret_buffer = CreateRetBuffer(returns.size());
    wgpu::raii::Buffer read_buffer = CreateReadBuffer(returns.size());
    wgpu::raii::BindGroup bind_group =
        CreateBindGroup(*bind_group_layout_, *arg_buffer, *ret_buffer);

    wgpu::raii::Queue queue = device_->getQueue();
    queue->writeBuffer(*arg_buffer, 0, arguments.data(), arguments.size());

    wgpu::raii::CommandEncoder command_encoder = CreateCommandEncoder();

    wgpu::raii::ComputePassEncoder pass_encoder =
        CreatePassEncoder(*command_encoder);
    pass_encoder->setPipeline(*pipeline);
    pass_encoder->setBindGroup(0, *bind_group, 0, nullptr);
    pass_encoder->dispatchWorkgroups(1, 1, 1);
    pass_encoder->end();

    /* Check for errors. */
    if (auto error = CheckError()) return std::unexpected(error.value());

    command_encoder->copyBufferToBuffer(
        *ret_buffer, 0, *read_buffer, 0, read_buffer->getSize());
    wgpu::raii::CommandBuffer command_buffer =
        CreateCommandBuffer(*command_encoder);

    /* Check for errors. */
    if (auto error = CheckError()) return std::unexpected(error.value());

    queue->submit(*command_buffer);
    /* Wait for the execution. */
    auto res = SyncReturn(*read_buffer, returns);
    if (!res) return std::unexpected(res.error());
    return {};
  }

 private:
  /* Variables */
  wgpu::raii::Device device_;
  std::optional<std::string> device_error_;
  wgpu::raii::BindGroupLayout bind_group_layout_;

  /* Functions */
  inline void InitDevice() {
    wgpu::raii::Adapter adapter = CreateWGPUAdapter();
    device_ = CreateWGPUDevice(*adapter, device_error_);
  }
  inline void InitBindGroupLayout() {
    std::array<wgpu::BindGroupLayoutEntry, 2> layout{ wgpu::Default,
                                                      wgpu::Default };
    wgpu::BindGroupLayoutEntry& arguments = layout[0];
    wgpu::BindGroupLayoutEntry& returns = layout[1];

    arguments.binding = 0;
    arguments.buffer.type = wgpu::BufferBindingType::Storage;
    arguments.visibility = wgpu::ShaderStage::Compute;
    arguments.texture.viewDimension = wgpu::TextureViewDimension::Undefined;

    returns.binding = 1;
    returns.buffer.type = wgpu::BufferBindingType::Storage;
    returns.visibility = wgpu::ShaderStage::Compute;

    wgpu::BindGroupLayoutDescriptor desc{};
    desc.entryCount = 2;
    desc.entries = layout.data();
    bind_group_layout_ = device_->createBindGroupLayout(desc);
  }

  inline wgpu::ShaderModule CreateShader(const std::string& code) {
    wgpu::ShaderSourceWGSL source{};
    source.chain.next = nullptr;
    source.chain.sType = wgpu::SType::ShaderSourceWGSL;
    source.code = wgpu::StringView(code);
    wgpu::ShaderModuleDescriptor desc{};
    desc.nextInChain = &source.chain;
    return device_->createShaderModule(desc);
  }

  inline wgpu::PipelineLayout CreatePipelineLayout() {
    wgpu::PipelineLayoutDescriptor desc{};
    desc.bindGroupLayoutCount = 1;
    WGPUBindGroupLayout bind_group_layout = *bind_group_layout_;
    desc.bindGroupLayouts = &bind_group_layout;
    return device_->createPipelineLayout(desc);
  }

  inline wgpu::ComputePipeline CreatePipeline(wgpu::ShaderModule& shader,
                                              wgpu::PipelineLayout& layout) {
    wgpu::ComputePipelineDescriptor desc{};
    desc.compute.constantCount = 0;
    desc.compute.constants = nullptr;
    desc.compute.entryPoint = wgpu::StringView("main");
    desc.compute.module = shader;
    desc.layout = layout;
    return device_->createComputePipeline(desc);
  }

  inline wgpu::Buffer CreateArgBuffer(size_t size) {
    wgpu::BufferDescriptor desc{};
    desc.mappedAtCreation = false;
    desc.size = size;
    desc.usage = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst;
    return device_->createBuffer(desc);
  }

  inline wgpu::Buffer CreateRetBuffer(size_t size) {
    wgpu::BufferDescriptor desc{};
    desc.mappedAtCreation = false;
    desc.size = size;
    desc.usage = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopySrc;
    return device_->createBuffer(desc);
  }

  inline wgpu::Buffer CreateReadBuffer(size_t size) {
    wgpu::BufferDescriptor desc{};
    desc.mappedAtCreation = false;
    desc.size = size;
    desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead;
    return device_->createBuffer(desc);
  }

  inline wgpu::BindGroup CreateBindGroup(wgpu::BindGroupLayout& layout,
                                         wgpu::Buffer& arg_buffer,
                                         wgpu::Buffer& ret_buffer) {
    std::array<wgpu::BindGroupEntry, 2> entries{ wgpu::Default, wgpu::Default };
    auto& arg_entry = entries[0];
    auto& ret_entry = entries[1];

    arg_entry.binding = 0;
    arg_entry.buffer = arg_buffer;
    arg_entry.offset = 0;
    arg_entry.size = arg_buffer.getSize();

    ret_entry.binding = 1;
    ret_entry.buffer = ret_buffer;
    ret_entry.offset = 0;
    ret_entry.size = ret_buffer.getSize();

    wgpu::BindGroupDescriptor desc{};
    desc.layout = layout;
    desc.entryCount = 2;
    desc.entries = entries.data();
    return device_->createBindGroup(desc);
  }

  inline wgpu::CommandEncoder CreateCommandEncoder() {
    wgpu::CommandEncoderDescriptor desc = wgpu::Default;
    return device_->createCommandEncoder(desc);
  }

  inline wgpu::ComputePassEncoder CreatePassEncoder(
      wgpu::CommandEncoder& encoder) {
    wgpu::ComputePassDescriptor desc{};
    desc.timestampWrites = nullptr;
    return encoder.beginComputePass(desc);
  }

  inline wgpu::CommandBuffer CreateCommandBuffer(
      wgpu::CommandEncoder& command_encoder) {
    wgpu::CommandBufferDescriptor desc{};
    desc.label = wgpu::StringView("CrystalGPU Command Buffer");
    return command_encoder.finish(desc);
  }

  inline std::expected<void, std::string> SyncReturn(
      wgpu::Buffer& read_buffer, std::span<std::byte>& ret) {
    bool flag = false;
    wgpu::BufferMapCallbackInfo info{};
    info.mode = wgpu::CallbackMode::AllowProcessEvents;
    info.userdata1 = &flag;
    info.callback = [](WGPUMapAsyncStatus status,
                       WGPUStringView message,
                       void* userdata1,
                       void*) {
      if (status == wgpu::MapAsyncStatus::Success) {
        bool& flag = *reinterpret_cast<bool*>(userdata1);
        flag = true;
      }
    };
    wgpuBufferMapAsync(
        read_buffer, wgpu::MapMode::Read, 0, read_buffer.getSize(), info);
    while (!flag) {
      auto res = context.Sync();
      if (!res) return std::unexpected(res.error());
    }
    std::memcpy(ret.data(),
                read_buffer.getConstMappedRange(0, read_buffer.getSize()),
                read_buffer.getSize());
    read_buffer.unmap();
    return {};
  }

  inline std::optional<std::string> CheckError() {
    if (device_error_) {
      std::string error = device_error_.value();
      device_error_ = {};
      return error;
    }
    if (!device_) return "Device not initialized.";
    return {};
  }
};

} // namespace crystal::gpu::impl

#endif