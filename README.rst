=============
 C++ library
=============

Utils
=====

Data Member Pointer
-------------------

Data Member Pointer contains information on how to access data member
in class. This type is useful for implementing intrusive containers.

Example
^^^^^^^

::

    class Item {
    private:
      int x_ = 0;
      int y_ = 0;
    public:
      using x_dmp = dmp<int Item::*, &Item::x_>;
    };

    Item i;
    int &x = Item::x_dmp::to_member(&i);

Containers
==========

Intrusive List
--------------

Simple linked-list container that supports removing of random elements
with O(1) complexity.

Size of a head and node elements is equal to size of two pointers.

============= ==========
Operation     Complexity
============= ==========
push_back     O(1)
push_front    O(1)
pop_front     O(1)
pop_back      O(1)
erase         O(1)
============= ==========

Example
^^^^^^^

::

    class Item {
    private:
      int x_ = 0;
      int y_ = 0;
      list_node item_idx_;
    
    public:
      using item_idx_dmp = dmp<list_node Item::*, &Item::item_idx_>;
    };

    list<Item::item_idx_dmp> items;
    Item i1;
    Item i2;
    items.push_back(i1);
    items.push_back(i2);

Intrusive Chain
---------------

Container with a stack like behaviour, but support removing of random
elements with O(1) complexity.

Size of a head element is equal to size of one pointer, and size of
node element is equal to size of two pointers.

============= ==========
Operation     Complexity
============= ==========
push          O(1)
pop           O(1)
erase         O(1)
============= ==========


Example
^^^^^^^

::

    class Item {
    private:
      int x_ = 0;
      int y_ = 0;
      chain_node item_idx_;
    
    public:
      using item_idx_dmp = dmp<chain_node Item::*, &Item::item_idx_>;
    };

    chain<Item::item_idx_dmp> items;
    Item i1;
    Item i2;
    items.push(i1);
    items.push(i2);


Intrusive Stack
---------------

Simple stack container.

Size of a head and node elements is equal to size of one pointer.

============= ==========
Operation     Complexity
============= ==========
push          O(1)
pop           O(1)
============= ==========

Example
^^^^^^^

::

    class Item {
    private:
      int x_ = 0;
      int y_ = 0;
      stack_node item_idx_;
    
    public:
      using item_idx_dmp = dmp<stack_node Item::*, &Item::item_idx_>;
    };

    stack<Item::item_idx_dmp> items;
    Item i1;
    Item i2;
    items.push(i1);
    items.push(i2);


Intrusive Queue
---------------

Simple queue container.

Size of a head element is equal to size of two pointer, and size of
node element is equal to size of one pointer.

============= ==========
Operation     Complexity
============= ==========
push          O(1)
pop           O(1)
============= ==========


Example
^^^^^^^

::

    class Item {
    private:
      int x_ = 0;
      int y_ = 0;
      queue_node item_idx_;
    
    public:
      using item_idx_dmp = dmp<queue_node Item::*, &Item::item_idx_>;
    };

    queue<Item::item_idx_dmp> items;
    Item i1;
    Item i2;
    items.push(i1);
    items.push(i2);

