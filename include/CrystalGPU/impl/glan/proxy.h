#ifndef CRYSTALGPU_IMPL_GLAN_PROXY_H_
#define CRYSTALGPU_IMPL_GLAN_PROXY_H_

#include <array>
#include <cassert>
#include <format>
#include <initializer_list>
#include <memory>

#include "dtype.h"
#include "expression.h"
#include "procedure.h"
#include "storage.h"
#include "type.h"
#include "utility.h"

namespace crystal::gpu::impl::glan {

using std::array, std::initializer_list, std::format;

template <AnyType ProxyType, AnyDType ProxyDType, AnyStorage ProxyStorage>
class Proxy {
 public:
  /* TypeDef for AnyExpr concept. */
  using Type = ProxyType;
  using DType = ProxyDType;

  /* Definition */
  Proxy() :
      sym_(Symbol{ .name = GenName("sym"),
                   .type_metadata = ProxyType{},
                   .dtype_metadata = ProxyDType{},
                   .storage_metadata = ProxyStorage{} }) {
    /* Append symbol to symbol table with metadata. */
    GlobalProc().SymbolTable().Insert(sym_);
    /* Local Definition */
    if constexpr (ProxyStorage::kDefLoc == DefLoc::Local)
      GlobalProc().PushDef(sym_);
    /* Function Parameter Definition */
    if constexpr (ProxyStorage::kDefLoc == DefLoc::FnParam)
      GlobalProc().CurrFn().Sig().params_.push_back(sym_);
  }
  /* Initialization */
  Proxy(const Proxy& rhs) :
      Proxy(static_cast<Expr<ProxyType, ProxyDType>>(rhs)) {
  }
  template <AnyType OtherType, AnyStorage OtherStorage>
  Proxy(const Proxy<OtherType, ProxyDType, OtherStorage>& rhs) :
      Proxy(static_cast<Expr<OtherType, ProxyDType>>(rhs)) {
  }
  template <AnyType OtherType>
  requires OtherType::kRVal
        && (ProxyType::kCompTimeConst ? OtherType::kCompTimeConst : true)
  Proxy(const Expr<OtherType, ProxyDType>& rhs) :
      Proxy() { // push symbol (lhs)
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
  auto operator=(Proxy<RType, ProxyDType, RStorage>& rhs) {
    return static_cast<Expr<ProxyType, ProxyDType>>(*this) =
               static_cast<Expr<RType, ProxyDType>>(rhs);
  }

  /* Subscript */
  auto operator[](size_t idx) {
    return static_cast<Expr<Type, DType>>(*this)[idx];
  }
  /* Swizzle */
  template <typename... Indicies>
  auto operator[](Indicies... indicies) {
    return static_cast<Expr<Type, DType>>(*this)[indicies...];
  }

  /* Conversion to Expr */
  operator Expr<ProxyType, ProxyDType>() {
    /* Procedure Operation */
    GlobalProc().Push(sym_.name);
    return {};
  }

 private:
  /* Member Variable */
  Symbol sym_;
};
static_assert(AnyExpr<Proxy<Type<>, DType<>, Storage<>>>);

}  // namespace crystal::gpu::impl::glan

#endif