#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_BLOCK_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_BLOCK_H_

#include <memory>
#include <string>
#include <list>

#include "statement.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::string;

class BLOCK : public STATEMENT {
 public:
  BLOCK() = default;
  BLOCK(const BLOCK& other) = delete;
  BLOCK(BLOCK&& other) = default;
  BLOCK& operator=(const BLOCK& rhs) = delete;
  BLOCK& operator=(BLOCK&& rhs) = delete;
  ~BLOCK() = default;

  operator string() const override {
    if (!STATEMENTS_.size()) return {};
    string RES;
    RES += "{\n";
    for (auto& ST : STATEMENTS_) {
      RES += *ST;
    }
    RES += "}\n";
    return RES;
  }
  template<typename T>
  T& APPEND_STATEMENT(T&& ST) {
    STATEMENTS_.push_back(std::make_unique<T>(ST));
    return *static_cast<T*>(STATEMENTS_.back().get());
  }
  BLOCK& APPEND_BLOCK() {
    STATEMENTS_.push_back(std::make_unique<BLOCK>());
    return *static_cast<BLOCK*>(STATEMENTS_.back().get());
  }

 private:
  std::list<std::unique_ptr<STATEMENT>> STATEMENTS_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif