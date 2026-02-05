#ifndef CRYSTALGPU_IMPL_GLAN_SYMBOL_TABLE_H_
#define CRYSTALGPU_IMPL_GLAN_SYMBOL_TABLE_H_

#include <vector>

#include "symbol.h"

namespace crystal::gpu::impl::glan {

using std::vector;

class SymbolTable {
 public:
  /* Modifiers */
  void Insert(Symbol SYM) {
    SYMBOLS_.push_back(SYM);
  }

 private:
  vector<Symbol> SYMBOLS_;
};

} // namespace crystal::gpu::impl::glan

#endif