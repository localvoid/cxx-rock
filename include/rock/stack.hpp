#ifndef _ROCK_STACK_HPP_
#define _ROCK_STACK_HPP_

/*
  Intrusive stack

  Root:
    first -> Node

  Node:
    next -> Node


  notes:
  - nodes cannot be removed at random order
 */


#include <cinttypes>
#include <iterator>


namespace rock {

class stack_node {
public:
  stack_node() {}
  stack_node(const stack_node&) = delete;
  stack_node &operator=(const stack_node&) = delete;

  bool is_linked() const noexcept {
    return !!next_;
  }

private:
  stack_node *next_ = nullptr;

  template<typename, typename> friend class stack_iterator;
  friend class stack_base;
};

class stack_base {
public:
  stack_base() {}
  stack_base(const stack_base &s) = delete;
  stack_base &operator=(const stack_base &s) = delete;

  bool is_empty() const noexcept { return !first_; }

protected:
  void push(stack_node &n) noexcept {
    n.next_ = first_;
    first_ = &n;
  }

  void pop() noexcept {
    assert(!is_empty());
    first_ = first_->next_;
  }

  stack_node &front() noexcept {
    assert(!is_empty());
    return *first_;
  }

  const stack_node &front() const noexcept {
    assert(!is_empty());
    return *first_;
  }

protected:
  stack_node *first_ = nullptr;
};

template<typename DMP, typename T>
class stack_iterator
  : public std::iterator<std::forward_iterator_tag, T, std::size_t> {
public:
  stack_iterator() {}
  stack_iterator(const stack_iterator &o) noexcept : node_(o.node_) {}
  stack_iterator &operator=(const stack_iterator &o) noexcept {
    node_ = o.node_;
    return *this;
  }

  stack_iterator &operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  stack_iterator &operator++(int) noexcept {
    stack_iterator result(*this);
    ++(*this);
    return result;
  }

  bool operator==(const stack_iterator &o) const noexcept {
    return node_ == o.node_;
  }
  bool operator!=(const stack_iterator &o) const noexcept {
    return node_ != o.node_;
  }

  T &operator*() const noexcept {
    return *DMP::to_container(node_);
  }
  T *operator->() const noexcept {
    return DMP::to_container(node_);
  }

private:
  stack_node *node_ = nullptr;
  stack_node *next_;

  explicit stack_iterator(stack_node *ptr) noexcept : node_(ptr) {}
  template<typename> friend class stack;
};


template<typename DMP>
class stack : public stack_base {
public:
  typedef typename DMP::container_type value_type;
  typedef value_type                  *pointer;
  typedef const value_type            *const_pointer;
  typedef value_type                  &reference;
  typedef const value_type            &const_reference;
  typedef std::size_t                  size_type;
  typedef std::size_t                  difference_type;

  typedef stack_iterator<DMP, value_type>       iterator;
  typedef stack_iterator<DMP, const value_type> const_iterator;


  iterator begin() noexcept {
    return iterator(first_);
  }

  iterator end() noexcept {
    return iterator();
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(first_);
  }

  const_iterator cend() const noexcept {
    return const_iterator();
  }


  void push(value_type &o) noexcept {
    stack_base::push_front(*DMP::to_member(&o));
  }

  value_type &pop() noexcept {
    return *DMP::to_container(stack_base::pop_front());
  }

  value_type &front() noexcept {
    return *DMP::to_container(&stack_base::front());
  }

  const value_type &front() const noexcept {
    return *DMP::to_container(&stack_base::front());
  }
};

}

#endif
