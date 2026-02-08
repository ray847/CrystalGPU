#ifndef CRYSTALGPU_IMPL_GLAN_PROXY_H_
#define CRYSTALGPU_IMPL_GLAN_PROXY_H_

#include <array>
#include <cassert>
#include <format>
#include <initializer_list>

#include "dtype.h"
#include "expression.h"
#include "procedure.h"
#include "storage.h"
#include "type.h"
#include "utility.h"


namespace crystal::gpu::impl::glan {

using std::array, std::initializer_list, std::format;

template <AnyType Type, AnyDType DType, AnyStorage Storage>
class Proxy {
 public:
  /* Definition */
  Proxy() :
      sym_(Symbol{ .name = GenName("sym"),
                   .type_metadata = Type{},
                   .dtype_metadata = DType{},
                   .storage_metadata = Storage{} }) {
    /* Append symbol to symbol table with metadata. */
    GlobalProc().SymbolTable().Insert(sym_);
    /* Local Definition */
    if constexpr (Storage::kDefLoc == DefLoc::Local) GlobalProc().PushDef(sym_);
    /* Function Parameter Definition */
    if constexpr (Storage::kDefLoc == DefLoc::FnParam)
      GlobalProc().CurrFn().Sig().params_.push_back(sym_);
  }
  /* Initialization */
  Proxy(const Proxy& rhs) : Proxy(static_cast<Expr<Type, DType>>(rhs)) {
  }
  template <AnyType OtherType, AnyStorage OtherStorage>
  Proxy(const Proxy<OtherType, DType, OtherStorage>& rhs) :
      Proxy(static_cast<Expr<OtherType, DType>>(rhs)) {
  }
  template <AnyType OtherType>
  requires OtherType::kRVal
        && (Type::kCompTimeConst ? OtherType::kCompTimeConst : true)
  Proxy(const Expr<OtherType, DType>& rhs) : Proxy() { // push symbol (lhs)
    GlobalProc().Push([](auto stk) {
      string lhs = stk.Pop();
      string rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
  }
  Proxy(Proxy&& other) = delete;
  Proxy& operator=(Proxy&& rhs) = delete;

  /* Assignment */
  template <AnyType RType, AnyStorage RStorage>
  auto operator=(Proxy<RType, DType, RStorage>& rhs) {
    return static_cast<Expr<Type, DType>>(*this) =
               static_cast<Expr<RType, DType>>(rhs);
  }

  /* Conversion to Expr */
  operator Expr<Type, DType>() const {
    /* Procedure Operation */
    GlobalProc().Push(sym_.name);
    return {};
  }

 private:
  /* Member Variable */
  Symbol sym_;
};

}  // namespace crystal::gpu::impl::glan

#endif