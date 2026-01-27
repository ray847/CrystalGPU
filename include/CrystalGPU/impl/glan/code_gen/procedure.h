#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_PROCEDURE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_PROCEDURE_H_

#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <format>
#include <list>

#include "statement.h"
#include "block.h"
#include "argument.h"
#include "return.h"
#include "function.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::string;
using std::format;

class PROCEDURE {
 public:
  static PROCEDURE* INSTANCE() {return INSTANCE_;}

  /* Constructor */
  PROCEDURE() {
    ARG::CLEAR();
    RET::CLEAR();
    INSTANCE_ = this;
  }

  /* Functions */
  template<typename T> requires std::derived_from<T, STATEMENT>
  void APPEND_STATEMENT(T&& ST) {
    CURR_STACK_->back()->APPEND_STATEMENT<T>(std::forward<T>(ST));
  }

  void BEGIN_BLOCK() {
    CURR_STACK_->push_back(&CURR_STACK_->back()->APPEND_BLOCK());
  }
  void END_BLOCK() {
    CURR_STACK_->pop_back();
    /* Switch to main if reach end of function. */
    if (CURR_STACK_ == &FUNCTION_STACK_ && CURR_STACK_->size() == 0) {
      CURR_STACK_ = &MAIN_STACK_;
    }
  }

  FUNCTION& BEGIN_FUNCTION(string RETURN_KEYWORD) {
    FUNCTIONS_.emplace_back(RETURN_KEYWORD);
    FUNCTION_STACK_.clear();
    FUNCTION_STACK_.push_back(&FUNCTIONS_.back().CODE_BLOCK_);
    CURR_STACK_ = &FUNCTION_STACK_;
    return FUNCTIONS_.back();
  }

  /**
   * Output the full code.
   */
  string operator[](uint32_t WORKGROUP_X, uint32_t WORKGROUP_Y,
                    uint32_t WORKGROUP_Z) const {
    string CODE;
    CODE += ARG::LIST();
    CODE += '\n';
    CODE += RET::LIST();
    CODE += '\n';
    CODE += format("@compute @workgroup_size({}, {}, {})\n", WORKGROUP_X,
                   WORKGROUP_Y, WORKGROUP_Z);
    CODE += format("fn main() ");
    CODE += MAIN_BLOCK_;
    CODE += '\n';
    for (auto& FUNCTION : FUNCTIONS_) {
      CODE += FUNCTION;
      CODE += '\n';
    }
    return CODE;
  }

 private:
  inline static PROCEDURE* INSTANCE_ = nullptr;

  std::list<ARG> MAIN_ARGS_;
  BLOCK MAIN_BLOCK_{};
  std::list<BLOCK*> MAIN_STACK_{&MAIN_BLOCK_};

  std::list<FUNCTION> FUNCTIONS_{};
  std::list<BLOCK*> FUNCTION_STACK_{};

  std::list<BLOCK*>* CURR_STACK_ = &MAIN_STACK_;
};

template<typename T> requires std::derived_from<T, STATEMENT>
inline void APPEND_STATEMENT(T&& ST) {
  if (PROCEDURE::INSTANCE())
    PROCEDURE::INSTANCE()->APPEND_STATEMENT(std::forward<T>(ST));
}
inline void BEGIN_BLOCK() {
  if (PROCEDURE::INSTANCE())
    PROCEDURE::INSTANCE()->BEGIN_BLOCK();
}
inline void END_BLOCK() {
  if (PROCEDURE::INSTANCE())
    PROCEDURE::INSTANCE()->END_BLOCK();
}
inline FUNCTION& BEGIN_FUNCTION(const string& RETURN_KEYWORD) {
  if (PROCEDURE::INSTANCE())
    return PROCEDURE::INSTANCE()->BEGIN_FUNCTION(
        std::forward<const string&>(RETURN_KEYWORD));
  throw std::runtime_error("Procedure not initialized yet.");
}

} // namespace crystal::gpu::impl::glan::code_gen

#endif