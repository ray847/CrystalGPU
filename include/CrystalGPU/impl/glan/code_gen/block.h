#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_BLOCK_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_BLOCK_H_

#include <string>
#include <format>
#include <vector>
#include <stdexcept>

namespace crystal::gpu::impl::glan::code_gen {

using std::string, std::format, std::vector, std::get;

class BLOCK {
 public:
  /* Constants */
  inline static const string kBEGIN_STR = "{ // BEGIN";
  inline static const string kEND_STR = "} // END";
  void PUSH(const string& STR) {
    STACK_.push_back(STR);
  }
  void PUSH(string&& STR) {
    STACK_.push_back(STR);
  }
  class STACK {
   public:
    STACK(vector<string>& REF) : REF_(REF) {
    }
    string POP() {
      auto TOP = REF_.back();
      REF_.pop_back();
      return TOP;
    }
    void PUSH(string&& STR) {
      REF_.push_back(STR);
    }

   private:
    vector<string>& REF_;
  };
  /**
   * Push an operator for the previous elements.
   */
  template<typename LAMBDA>
  void PUSH(LAMBDA OP) {
    OP(STACK(STACK_));
  }
  void PUSH() {
    PUSH([](auto STACK) { STACK.PUSH(STACK.POP() + kBEGIN_STR); });
    DEPTH_++;
  }
  void POP() {
    if (DEPTH_ == 0) throw std::runtime_error("Already at most outer scope.");
    STACK_.push_back(kEND_STR);
    DEPTH_--;
  }
  int DEPTH() {
    return DEPTH_;
  }
  operator string() const {
    string RES = "{\n";
    int INDENT = 1; // indenting for prettier code
    for (auto STR : STACK_) {
      /* Indents */
      for (int i = 0; i < INDENT; ++i) RES += '\t';
      /* Content */
      if (STR.ends_with(kBEGIN_STR)) {
        RES += STR + '\n';
        INDENT++;
      } else if (STR.ends_with(kEND_STR)) {
        RES += STR + '\n';
        INDENT--;
      } else {
        RES += STR + ";\n";
      }
    }
    RES += "}\n";
    return RES;
  }

 private:
  vector<string> STACK_;
  int DEPTH_ = 0;
};
} // namespace crystal::gpu::impl::glan::code_gen

#endif