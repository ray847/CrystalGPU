#ifndef CRYSTALGPU_IMPL_GLAN_PROCEDURE_H_
#define CRYSTALGPU_IMPL_GLAN_PROCEDURE_H_

#include <string>
#include <string_view>
#include <vector>
#include <format>

#include "function.h"
#include "symbol_table.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::vector, std::string, std::string_view, std::format;

class Proc {
 public:
  inline void Push(const string& S) {
    curr_fn_->Def().Push(S);
  }
  inline void Push(string&& S) {
    curr_fn_->Def().Push(S);
  }
  inline void Push(string_view S) {
    curr_fn_->Def().Push(S);
  }
  template <typename lambda>
  inline void Push(lambda op) {
    curr_fn_->Def().Push(op);
  }
  inline void Push() {
    curr_fn_->Def().Push();
  }
  inline void Pop() {
    if (curr_fn_->Def().Depth() == 0) curr_fn_ = &main_; //  go back to main
    else curr_fn_->Def().POP();
  }
  inline void PushDef(const Symbol& symbol) {
    Push(format("{} {}: {}",
                symbol.behavior.def_keyword,
                symbol.name,
                symbol.type.wgsl_keyword));
  }
  /**
   * Begin a function definition in the procedure.
   *
   * To end a function definition and return to the definition for main, call
   * `Pop()` with no arguments at the end.
   */
  inline void BeginFn(Type T) {
    fns_.emplace_back(T);
  }
  inline auto& SymbolTable() {
    return symbol_table_;
  }
  operator string() const {
    string code;
    code += main_;
    for (auto& fn : fns_) code += fn;
    return code;
  }

 private:
  class SymbolTable symbol_table_;
  /* Main Function. */
  class Fn main_ {
    VOID{}, "MAIN"
  };
  /* Custom Functions */
  vector<class Fn> fns_;
  /* Pointer to the current function. */
  Fn* curr_fn_ = &main_;
};

} // namespace crystal::gpu::impl::glan

#endif