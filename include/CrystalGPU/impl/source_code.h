#ifndef CRYSTALGPU_IMPL_SOURCE_CODE_H_
#define CRYSTALGPU_IMPL_SOURCE_CODE_H_

#include <string>

namespace crystal::gpu::impl {

using std::string;

enum class SrcType {
  WGSL
};

class Src {
 public:
  const SrcType type_;
  const string code_;
  template <SrcType type>
  Src(string code) : type_(type), code_(code) {
  }
};
} // namespace crystal::gpu::impl

#endif