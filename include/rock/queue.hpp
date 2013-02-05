#ifndef _ROCK_QUEUE_HPP_
#define _ROCK_QUEUE_HPP_

/*
  Intrusive Queue

  Root:
   first -> Node
   last  -> Node

  Node:
   next  -> Node


  notes:
  - nodes cannot be removed at random order
 */


#include <cinttypes>
#include <iterator>


namespace rock {

class queue_node {
public:
  queue_node() {}
  queue_node(const queue_node&) = delete;
  queue_node &operator=(const queue_node&) = delete;


  bool is_linked() const noexcept {
    return !!next_;
  }

private:
  queue_node *next_;

  template<typename, typename> friend class queue_iterator;
  friend class queue_base;
};

class queue_base {
public:
  queue_base(const queue_base &s) = delete;
  queue_base &operator=(const queue_base &s) = delete;


  bool is_empty() const noexcept {
    return !first_;
  }

protected:
  void push(queue_node &n) noexcept {
    if (last_) {
      last_->next_ = &n;
    }
    else {
      first_ = &n;
    }
    n.next_ = nullptr;
    last_ = &n;
  }

  queue_node &pop() noexcept {
    assert(!is_empty());

    queue_node *first = first_;
    first_ = first->next_;
    return *first;
  }

  queue_node &front() noexcept {
    assert(!is_empty());
    return *first_;
  }

  const queue_node &front() const noexcept {
    assert(!is_empty());
    return *first_;
  }

  queue_node &back() noexcept {
    assert(!is_empty());
    return *last_;
  }

  const queue_node &back() const noexcept {
    assert(!is_empty());
    return *last_;
  }

protected:
  queue_node *first_ = nullptr;
  queue_node *last_  = nullptr;
};

template<typename DMP, typename T>
class queue_iterator : public std::iterator<std::forward_iterator_tag, T, std::size_t> {
public:
  queue_iterator(const queue_iterator &o) : node_(o.node_) {}
  queue_iterator &operator=(const queue_iterator &o) {
    node_ = o.node_;
    return *this;
  }


  queue_iterator &operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  queue_iterator &operator++(int) noexcept {
    queue_iterator result(*this);
    ++(*this);
    return result;
  }

  bool operator==(const queue_iterator &o) const noexcept {
    return node_ == o.node_;
  }
  bool operator!=(const queue_iterator &o) const noexcept {
    return node_ != o.node_;
  }

  T &operator*() const noexcept {
    return *DMP::to_container(node_);
  }
  T *operator->() const noexcept {
    return DMP::to_container(node_);
  }

private:
  queue_node *node_;

  explicit queue_iterator(queue_node *ptr) noexcept : node_(ptr) {}
  template<typename> friend class queue;
};


template<typename DMP>
class queue : public queue_base {
public:
  typedef typename DMP::container_type value_type;
  typedef value_type                  *pointer;
  typedef const value_type            *const_pointer;
  typedef value_type                  &reference;
  typedef const value_type            &const_reference;
  typedef std::size_t                  size_type;
  typedef std::size_t                  difference_type;

  typedef queue_iterator<DMP, value_type>       iterator;
  typedef queue_iterator<DMP, const value_type> const_iterator;

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
    queue_base::push(*DMP::to_member(&o));
  }
  value_type &pop() noexcept {
    return *DMP::to_container(&queue_base::pop());
  }


  value_type &front() noexcept {
    return *DMP::to_container(&queue_base::front());
  }

  const value_type &front() const noexcept {
    return *DMP::to_container(&queue_base::front());
  }

  value_type &back() noexcept {
    return *DMP::to_container(&queue_base::back());
  }

  const value_type &back() const noexcept {
    return *DMP::to_container(&queue_base::back());
  }
};

}

#endif
