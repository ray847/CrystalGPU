#ifndef CRYSTALGPU_IMPL_GLAN_STATEMENT_H_
#define CRYSTALGPU_IMPL_GLAN_STATEMENT_H_

#include <concepts>
#include <format>
#include <string>

#include "code_gen/expression.h"
#include "code_gen/procedure.h"
#include "code_gen/statement.h"
#include "code_gen/variable.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format;

inline void STATE_EXPR(const code_gen::EXPR& EXPR) {
  class EXPR_STATEMENT : public code_gen::STATEMENT {
   public:
    EXPR_STATEMENT(const code_gen::EXPR& EXPR) : EXPR_(EXPR) {}
    operator string() const override { return format("{};\n", EXPR_); }

   private:
    string EXPR_;
  };
  code_gen::APPEND_STATEMENT(EXPR_STATEMENT{EXPR});
}

inline void DECLARE_VAR(const code_gen::VAR& VAR) {
  class DECLARE_VAR_STATEMENT : public code_gen::STATEMENT {
   public:
    DECLARE_VAR_STATEMENT(const code_gen::VAR& VAR)
        : STR_(format("var {}: {};\n", VAR.NAME(), VAR.KEYWORD())) {}
    operator string() const { return STR_; }

   private:
    const string STR_;
  };
  code_gen::APPEND_STATEMENT(
      DECLARE_VAR_STATEMENT{*static_cast<const code_gen::VAR*>(&VAR)});
}

template <typename T>
  requires(std::derived_from<T, code_gen::VAR> ||
           std::derived_from<T, code_gen::ARG> ||
           std::derived_from<T, code_gen::RET>)
inline void ASSIGN(const T& LHS, const code_gen::EXPR& RHS) {
  class ASSIGN_STATEMENT : public code_gen::STATEMENT {
   public:
    ASSIGN_STATEMENT(const code_gen::VAR& LHS, const code_gen::EXPR& RHS)
        : STR_(format(
              "{} = {};\n", static_cast<string>(LHS),
              static_cast<string>(*static_cast<const code_gen::EXPR*>(&RHS)))) {
    }
    ASSIGN_STATEMENT(const code_gen::ARG& LHS, const code_gen::EXPR& RHS)
        : STR_(format(
              "{} = {};\n", static_cast<string>(LHS),
              static_cast<string>(*static_cast<const code_gen::EXPR*>(&RHS)))) {
    }
    ASSIGN_STATEMENT(const code_gen::RET& LHS, const code_gen::EXPR& RHS)
        : STR_(format(
              "{} = {};\n", static_cast<string>(LHS),
              static_cast<string>(*static_cast<const code_gen::EXPR*>(&RHS)))) {
    }
    operator string() const override { return STR_; }

   private:
    string STR_;
  };
  if constexpr (std::derived_from<T, code_gen::VAR>) {
    code_gen::APPEND_STATEMENT(
        ASSIGN_STATEMENT{*static_cast<const code_gen::VAR*>(&LHS),
                         *static_cast<const code_gen::EXPR*>(&RHS)});
  } else if constexpr (std::derived_from<T, code_gen::ARG>) {
    code_gen::APPEND_STATEMENT(
        ASSIGN_STATEMENT{*static_cast<const code_gen::ARG*>(&LHS),
                         *static_cast<const code_gen::EXPR*>(&RHS)});
  } else if constexpr (std::derived_from<T, code_gen::RET>) {
    code_gen::APPEND_STATEMENT(
        ASSIGN_STATEMENT{*static_cast<const code_gen::RET*>(&LHS),
                         *static_cast<const code_gen::EXPR*>(&RHS)});
  }
}

inline void RETURN(code_gen::EXPR& EXPR) {
  class RETURN_STATEMENT : public code_gen::STATEMENT {
   public:
    RETURN_STATEMENT(code_gen::EXPR& EXPR) : EXPR_(EXPR) {}
    operator string() const override { return format("return {};\n", EXPR_); }

   private:
    string EXPR_;
  };
  code_gen::APPEND_STATEMENT(RETURN_STATEMENT(EXPR));
}

} // namespace crystal::gpu::impl::glan

#endif