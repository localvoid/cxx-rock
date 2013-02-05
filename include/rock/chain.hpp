#ifndef _ROCK_CHAIN_HPP_
#define _ROCK_CHAIN_HPP_

/*
  Intrusive chain (Stack)

  Root:
    first_

  Node:
    next -> Node
    pprev -> *Node

*/

#include <cinttypes>
#include <iterator>

namespace rock {

class chain_node {
public:
  chain_node() {}
  chain_node(const chain_node&) = delete;
  chain_node &operator=(const chain_node&) = delete;

  bool linked() noexcept { return !!pprev_; }

  void unlink() noexcept {
    assert(linked());

    chain_node *next = next_;
    chain_node **pprev = pprev_;
    *pprev_ = next;
    if (next) {
      next->pprev_ = pprev;
    }
  }

  void replace(chain_node &n) noexcept {
    assert(linked());

    chain_node *next = next_;
    chain_node **pprev = pprev_;
    n.next_ = next;
    next->pprev_ = &n.next_;
    *pprev = &n;
  }

  void prepend(chain_node &n) noexcept {
    pprev_ = n.pprev_;
    next_ = &n;
    n.pprev_ = &next_;
    *(n.pprev_) = this;
  }

  void append(chain_node &n) noexcept {
    n.next_ = next_;
    next_ = &n;
    n.pprev_ = &next_;
    if (n.next_) {
      n.next_->pprev_ = &n.next_;
    }
  }

private:
  chain_node *next_ = nullptr;
  chain_node **pprev_;

template<typename, typename> friend class chain_iterator;
  friend class chain_base;
};

class chain_base {
public:
  chain_base() : first_(nullptr) {};
  chain_base(const chain_base&) = delete;
  chain_base &operator=(const chain_base&) = delete;

  bool empty() const noexcept { return !first_; }

protected:
  void push(chain_node &n) noexcept {
    chain_node *first = first_;
    n.next_ = first;
    if (first) {
      first->pprev_ = &n.next_;
    }
    first_ = &n;
    n.pprev_ = &first_;
  }

  chain_node &pop() noexcept {
    assert(!empty());

    chain_node *first = first_;
    first->unlink();
    return *first;
  }

  chain_node &front() const noexcept {
    assert(!empty());
    return *first_;
  }

  void move(chain_base &old) noexcept {
    first_ = old.first_;
    if (first_) {
      first_->pprev_ = &first_;
    }
    old.first_ = nullptr;
  }


  chain_node *first_ = nullptr;
};

template<typename DMP, typename T>
class chain_iterator : public std::iterator<std::forward_iterator_tag, T, std::size_t> {
public:
  chain_iterator() noexcept {}
  chain_iterator(const chain_iterator &o) noexcept : node_(o.node_) {}
  chain_iterator &operator=(const chain_iterator &o) noexcept {
    node_ = o.node_;
    return *this;
  }

  chain_iterator &operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  chain_iterator operator++(int) noexcept {
    chain_iterator result(*this);
    ++(*this);
    return result;
  }

  bool operator==(const chain_iterator &o) const noexcept {
    return node_ == o.node_;
  }
  bool operator!=(const chain_iterator &o) const noexcept {
    return node_ != o.node_;
  }

  T &operator*() const noexcept {
    return *DMP::to_container(node_);
  }

  T *operator->() const noexcept {
    return DMP::to_container(node_);
  }

protected:
  chain_node *node_ = nullptr;

  explicit chain_iterator(chain_node *ptr) noexcept : node_(ptr) {}
  template<typename> friend class chain;
};


template<typename DMP>
class chain : public chain_base {
public:
  typedef typename DMP::container_type value_type;
  typedef value_type                  *pointer;
  typedef const value_type            *const_pointer;
  typedef value_type                  &reference;
  typedef const value_type            &const_reference;
  typedef std::size_t                  size_type;
  typedef std::size_t                  difference_type;

  using iterator       = chain_iterator<DMP, value_type>;
  using const_iterator = chain_iterator<DMP, const value_type>;


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


  void push(reference o) noexcept {
    chain_base::push(*DMP::to_member(&o));
  }
  reference pop() noexcept {
    return *DMP::to_container(&chain_base::pop());
  }

  reference front() noexcept {
    return *DMP::to_container(&chain_base::front());
  }
  const_reference front() const noexcept {
    return *DMP::to_container(&chain_base::front());
  }


  void erase(reference o) noexcept {
    DMP::to_member(&o)->unlink();
  }
  void erase(iterator i) noexcept {
    i.node_->unlink();
  }
};

}

#endif
