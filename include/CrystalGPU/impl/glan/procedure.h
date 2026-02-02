#ifndef CRYSTALGPU_IMPL_GLAN_PROCEDURE_H_
#define CRYSTALGPU_IMPL_GLAN_PROCEDURE_H_

#include <string>
#include <memory>
#include <expected>

#include "CrystalGPU/impl/glan/code_gen/function.h"
#include "code_gen/procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string, std::unique_ptr, std::expected;

struct BLOCK_BEGIN_RETURN;

/**
 * A GPU procedure (corresponds to a GPU pipeline).
 *
 * This class is a wrapper around the class `code_gen::PROCEDURE`, implementing
 * a thread local singleton pattern (thread local for the CPU).
 * This class exposes the following interfaces for the other classes in this
 * hierarchy: `Push()` (overloaded), `Pop()`.
 * This class is furthur specialized to create the compute procedure class and
 * the render procedure class.
 */
class PROCEDURE {
 public:
  template <typename T>
  requires std::is_same_v<T, BLOCK_BEGIN_RETURN>
  PROCEDURE(T) {
    /* Check if there is another instance in place. */
    if (INSTANCE_) return;
    INSTANCE_ = std::make_unique<code_gen::PROCEDURE>();
  }
  ~PROCEDURE() {
    INSTANCE_ = nullptr;
  }
  expected<string, string> TO_CODE() const {
    if (INSTANCE_) return INSTANCE_->operator string();
    return std::unexpected("Procedure not initialized.");
  }

  static void PUSH(const string& STR) {
    if (INSTANCE_) INSTANCE_->PUSH(STR);
  }
  static void PUSH(string&& STR) {
    if (INSTANCE_) INSTANCE_->PUSH(STR);
  }
  static void PUSH(string(*OP)(const string&)) {
    if (INSTANCE_) INSTANCE_->PUSH(OP);
  }
  static void PUSH(string(*OP)(const string&, const string&)) {
    if (INSTANCE_) INSTANCE_->PUSH(OP);
  }
  static void PUSH(string(*OP)(const string&, const string&, const string&)) {
    if (INSTANCE_) INSTANCE_->PUSH(OP);
  }
  static void PUSH() {
    if (INSTANCE_) INSTANCE_->PUSH();
  }
  static void POP() {
    if (INSTANCE_) INSTANCE_->POP();
  }
  static void DEFINE_TUPLE(size_t TAG) {
    if (INSTANCE_) INSTANCE_->DEFINE_TUPLE(TAG);
  }
  template <ANY_TYPE T>
  static size_t BEGIN_FUNCTION() {
    if (INSTANCE_) return INSTANCE_->BEGIN_FUNCTION(T::CODE_GEN_TYPE);
    return 0;
  }
  static const code_gen::FUNCTION* GET_FUNCTION(size_t IDX) {
    if (INSTANCE_) return &INSTANCE_->GET_FUNCTION(IDX);
    return nullptr;
  }

 private:
  inline thread_local static unique_ptr<code_gen::PROCEDURE> INSTANCE_{};
};

} // namespace crystal::gpu::impl::glan

#endif