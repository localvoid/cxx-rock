#ifndef _ROCK_DELEGATE_H_
#define _ROCK_DELEGATE_H_

/*
  Lightweight delegates (size of 2 pointers)
 */

namespace rock {

class delegate_base {
public:
  delegate_base() {}

  delegate_base(const delegate_base &r)
    : object_(r.object_),
      function_(r.function_) {}

  delegate_base &operator=(const delegate_base &r) {
    object_ = r.object_;
    function_ = r.function_;
    return *this;
  }

public:
  void clear() noexcept {
    object_   = nullptr;
    function_ = nullptr;
  }

  explicit operator bool() const noexcept {
    return function_ == nullptr;
  }

  bool operator<(const delegate_base &r) const noexcept {
    return is_less(r);
  }
  bool operator>(const delegate_base &r) const noexcept {
    return r.is_less(*this);
  }

protected:
  bool is_equal(const delegate_base &r) const noexcept {
    return (object_ == r.object_) && (function_ == r.function_);
  }

  bool is_less(const delegate_base &r) const noexcept {
    if (object_ != r.object_)
      return object_ < r.object_;
    return function_ < r.function_;
  }

protected:
  typedef void (*generic_function)(void *);

  void             *object_   = nullptr;
  generic_function  function_ = nullptr;
};


template<typename R, typename ... Args>
class delegate;

template<typename R, typename ... Args>
class delegate<R (Args ...)> : public delegate_base {
public:
  template<typename X, typename Y>
  void bind(Y *object, R (X::*method)(Args ...)) noexcept {
    object_ = reinterpret_cast<void*>(object);
    function_ = reinterpret_cast<generic_function>(object->*method);
  }

  template<typename X, typename Y>
  void bind(Y *object, X function) noexcept {
    object_ = reinterpret_cast<void*>(object);
    function_ = reinterpret_cast<generic_function>(function);
  }

  R operator()(Args ... args) const noexcept {
    return reinterpret_cast<__attribute__(thiscall) R (*)(void*, Args...)>(function)(object_, args...);
  }
};

}

#endif
