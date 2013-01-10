#ifndef _ROCK_UTILS_H_
#define _ROCK_UTILS_H_


#include <cstddef>
#include <cstdint>


namespace rock {

/*
 */
template<class T, std::size_t N>
constexpr std::size_t array_size(const T (&)[N]) {
  return N;
}


/*
 */
template<typename C, typename M>
std::ptrdiff_t offset_of(const M C::* ptr_to_member) {
   const C *const container = 0;
   const char *const member =
     reinterpret_cast<const char*>(&(container->*ptr_to_member));
   return std::ptrdiff_t(member - reinterpret_cast<const char*>(container));
}

template<typename C, typename M>
C *container_of(M *member, const M C::* ptr_to_member) {
  return reinterpret_cast<C*>(reinterpret_cast<char*>(member) -
                              offset_of(ptr_to_member));
}

template<class C, class M>
const C *container_of(const M *member, const M C::* ptr_to_member) {
  return reinterpret_cast<const C*>(reinterpret_cast<const char*>(member) -
                                    offset_of(ptr_to_member));
}


/*
  Data Member Pointer

  used with intrusive containers
 */
template<typename T, T P>
class dmp;

template<typename C, typename T, T C::*P>
class dmp<T C::*, P> {
public:
  using container_type = C;
  using member_type    = T;

  static constexpr T C::*pointer_to_member = P;

public:
  constexpr static T *to_member(C *container) noexcept {
    return &(container->*P);
  }
  constexpr static const T *to_member(const C *container) noexcept {
    return &(container->*P);
  }
  constexpr static C *to_container(T *ptr) noexcept {
    return container_of(ptr, P);
  }
  constexpr static const C *to_container(const T *ptr) noexcept {
    return container_of(ptr, P);
  }
};

}

#endif
