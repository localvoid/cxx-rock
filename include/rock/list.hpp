#ifndef _ROCK_LIST_HPP_
#define _ROCK_LIST_HPP_

/*
  Intrusive List

  Root same as Node:
    prev -> Node
    next -> Node
 */


#include <iterator>


namespace rock {

class list_node {
public:
  list_node(const list_node&) = delete;
  list_node &operator=(const list_node&) = delete;

  list_node() noexcept {}

  ~list_node() {
    unlink();
  }

  void unlink() noexcept {
    next_->prev_ = prev_;
    prev_->next_ = next_;
  }

protected:
  static void add_(list_node &n, list_node &prev, list_node &next) noexcept {
    next.prev_ = &n;
    n.next_ = &next;
    n.prev_ = &prev;
    prev.next_ = &n;
  }

  void prepend_(list_node &n) noexcept {
    add_(n, *this, *this->next_);
  }

  void append_(list_node &n) noexcept {
    add_(n, *this->prev_, *this);
  }

  void replace_(list_node &n) {
    n.next_ = next_;
    n.next_->prev_ = &n;
    n.prev_ = prev_;
    n.prev_->next_ = &n;
  }

protected:
  list_node *next_ = this;
  list_node *prev_ = this;

  template<typename, typename> friend class ListIterator;
};


class list_base : public list_node {
public:
  list_base() noexcept : list_node() {}
  list_base(const list_base&) = delete;
  list_base &operator=(const list_base&) = delete;

  bool empty() const noexcept {
    return next_ == this;
  }

protected:
  void push_front(list_node &n) noexcept {
    prepend_(n);
  }

  void push_back(list_node &n) noexcept {
    append_(n);
  }

  list_node &pop_front() noexcept {
    assert(!empty());
    list_node *first = next_;
    first->unlink();
    return *first;
  }

  list_node &pop_back() noexcept {
    assert(!empty());
    list_node *last = prev_;
    last->unlink();
    return *last;
  }

  list_node &front() const noexcept {
    assert(!empty());
    return *next_;
  }

  list_node &back() const noexcept {
    return *prev_;
  }
};


template<typename DMP, typename T>
class ListIterator :
    public std::iterator<std::bidirectional_iterator_tag, T, std::size_t> {
public:
  ListIterator() noexcept : node_(nullptr) {}
  ListIterator(const ListIterator &o) noexcept : node_(o.node_) {}
  ListIterator &operator=(const ListIterator &o) noexcept {
    node_ = o.node_;
    return *this;
  }

  ListIterator &operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  ListIterator operator++(int) noexcept {
    ListIterator result(*this);
    ++(*this);
    return result;
  }

  ListIterator &operator--() noexcept {
    node_ = node_->prev_;
    return *this;
  }

  ListIterator operator--(int) noexcept {
    ListIterator result(*this);
    --(*this);
    return result;
  }

  bool operator==(const ListIterator &o) const noexcept {
    return node_ == o.node_;
  }
  bool operator!=(const ListIterator &o) const noexcept {
    return node_ != o.node_;
  }

  T &operator*() const noexcept {
    return *DMP::to_container(node_);
  }
  T *operator->() const noexcept {
    return DMP::to_container(node_);
  }

private:
  list_node *node_;

  explicit ListIterator(list_node *ptr) noexcept : node_(ptr) {}
  template<typename> friend class List;
};


template<typename DMP>
class list : public list_base {
public:
  using value_type      = typename DMP::container_type;
  using pointer         = value_type*;
  using const_pointer   = const value_type*;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using size_type       = std::size_t;
  using difference_type = std::size_t;

  using iterator               = ListIterator<DMP, value_type>;
  using const_iterator         = ListIterator<DMP, const value_type>;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;


  list(const list&) = delete;
  list &operator=(const list&) = delete;


  list() noexcept : list_base() {}


  iterator begin() noexcept {
    return iterator(next_);
  }
  iterator end() noexcept {
    return iterator(this);
  }
  const_iterator cbegin() const noexcept {
    return const_iterator(next_);
  }
  const_iterator cend() const noexcept {
    return const_iterator(this);
  }

  iterator rbegin() noexcept {
    return reverse_iterator(prev_);
  }
  iterator rend() -> iterator noexcept {
    return reverse_iterator(this);
  }
  const_iterator crbegin() const noexcept {
    return const_reverse_iterator(prev_);
  }
  const_iterator crend() const noexcept {
    return const_reverse_iterator(this);
  }


  reference front() noexcept {
    return *DMP::to_container(&list_base::front());
  }
  const_reference front() const noexcept {
    return *DMP::to_container(&list_base::front());
  }
  reference back() noexcept {
    return *DMP::to_container(&list_base::back());
  }
  const_reference back() const noexcept {
    return *DMP::to_container(&list_base::back());
  }


  void push_front(reference o) noexcept {
    list_base::push_front(*DMP::to_member(&o));
  }
  void push_back(reference o) noexcept {
    list_base::push_back(*DMP::to_member(&o));
  }
  reference pop_front() noexcept {
    return *DMP::to_container(&list_base::pop_front());
  }
  reference pop_back() noexcept {
    return *DMP::to_container(&list_base::pop_back());
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
