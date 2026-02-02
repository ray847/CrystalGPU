#ifndef CRYSTALGPU_IMPL_GLAN_BLOCK_H_
#define CRYSTALGPU_IMPL_GLAN_BLOCK_H_

#include "procedure.h"

namespace crystal::gpu::impl::glan {

struct BLOCK_BEGIN_RETURN {};

/**
 * Indicate the beginning of something (e.g. Procedure, Function, Code Block,
 * etc.).
 *
 * This function does nothing other that returning a beginning object. The
 * return is meant to be assigned to a object to begin a code block, e.g.:
 * `PROCEDURE proc = BEGIN();`
 */
[[nodiscard]] inline BLOCK_BEGIN_RETURN BEGIN() {
  return {};
}

/**
 * Indicate the end of something (e.g. Procedure, Function, Code Block, etc.).
 */
inline void END() {
  PROCEDURE::POP();
}

} // namespace crystal::gpu::impl::glan

#endif