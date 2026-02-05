#ifndef CRYSTALGPU_IMPL_GLAN_BLOCK_H_
#define CRYSTALGPU_IMPL_GLAN_BLOCK_H_

#include <vector>
#include <string>
#include <string_view>
#include <format>

namespace crystal::gpu::impl::glan {

using std::string, std::vector, std::string_view, std::format;

class BLK {
 public:
  /* Custom type for manipulating the string stack. */
  class StrStk {
   public:
    /* Constructor */
    StrStk(vector<string>& STACK) : STACK_REF_(STACK) {
    }
    /* Functions */
    void Push(const string& S) {
      STACK_REF_.push_back(S);
    }
    string Pop() {
      auto S = STACK_REF_.back();
      STACK_REF_.pop_back();
      return S;
    }

   private:
    /* Variables */
    vector<string>& STACK_REF_;
  };
  void Push(const string& S) {
    stk_.push_back(S);
  }
  void Push(string&& S) {
    stk_.push_back(S);
  }
  void Push(string_view S) {
    stk_.push_back(string{S});
  }
  template <typename lambda>
  void Push(lambda op) {
    op(StrStk{stk_});
  }
  void Push() {
    Push(kBeginToken);
    depth_++;
  }
  void POP() {
    Push(kEndToken);
    depth_--;
  }
  operator string() const {
    string code = "{\n";
    for (auto& LINE : stk_) {
      if (LINE.ends_with(kBeginToken) || LINE.ends_with(kEndToken))
        code += LINE + "\n"; // not a statement
      else
        code += LINE + ";\n"; // is a statement
    }
    code += "}\n";
    return code;
  }
  size_t Depth() const {return depth_;}

 private:
  inline static constexpr string_view kBeginToken = "{ // BEGIN";
  inline static constexpr string_view kEndToken = "} // END";

  vector<string> stk_;
  /* Depth of the current statement. */
  size_t depth_ = 0;
};

} // namespace crystal::gpu::impl::glan

#endif