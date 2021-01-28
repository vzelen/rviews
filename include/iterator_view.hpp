#pragma once
#include <iostream>
#include <type_traits>

#include <map>

#include "base_view.hpp"

namespace rviews {

template <typename Iterator>
class associate_iterator_view : public Iterator {
   public:
    using base_iterator = Iterator;
    associate_iterator_view(Iterator const& it) : Iterator{it} {}

    VIEW_API associate_iterator_view
    operator+(typename Iterator::difference_type n) {
        // return Iterator::operator+(n); does not work for non-random
        // iterators, so do constexpr if here and dispatch by their tags
        // or use advance.
        auto it = *this;
        std::advance(it, n);
        return it;
    }

    auto& operator*() { return Iterator::operator*(); }
};

template <typename Iterator>
class sequential_iterator_view : public Iterator {
   public:
    using base_iterator = Iterator;

    sequential_iterator_view(Iterator const& it) : Iterator{it} {}
    sequential_iterator_view(Iterator& it) : Iterator{it} {}

    sequential_iterator_view operator+(typename Iterator::difference_type n) {
        // return Iterator::operator+(n); does not work for non-random
        // iterators, so do constexpr if here and dispatch by their tags
        // or use advance.
        auto it = *this;
        std::advance(it, n);
        return it;
    }

    template <typename Container, typename ContainerIterator,
              typename ViewContainer>
    sequential_iterator_view(Container&& c, ContainerIterator&& it,
                             ViewContainer&& v)
        : Iterator{get_view_iterator(c, it, v)} {}

    /* Give a container, view container, and an iterator to a container,
       obtain the iterator to a view container that points to the same
       element, to which the original iterator points to */
    template <typename Container, typename ContainerIterator,
              typename ViewContainer>
    auto get_view_iterator(Container&& c, ContainerIterator&& it,
                           ViewContainer&& vc) {
        auto d = std::distance(std::begin(c), it);
        auto vit = std::begin(vc);
        std::advance(vit, d);
        return vit;
    }

    /* ViewContainer stores pointers to target objects, its
     * native ViewContainer::iterator would do the same without current class.
     * Here, introduce a new iterator class on top of ViewContainer::iterator,
     * that will dereference the pointers and return the underlying objects.
     * That does not mean that current class stores real objects -
     * it still stores pointers, but dereferences them in the last moment.
     */
    auto& operator*() { return *(Iterator::operator*()); }

    auto&& get() { return Iterator::operator*(); }

    /* Given an iterator from a Container class (e.g. an iterator from the
     * container we are viewing onto), convert it to a ViewContainer::iterator
     */
    template <typename TargetIterator>
    TargetIterator cast_to_target() {
        auto ptr = this->Iterator::operator*();
        // auto ptr = this->Iterator::data();
        return TargetIterator(ptr);
    }
};

template <class T>
struct is_const_pointer_helper : std::false_type {};

template <class T>
struct is_const_pointer_helper<const T*> : std::true_type {};

template <class Iterator>
struct is_const_iterator
    : is_const_pointer_helper<
          typename std::iterator_traits<Iterator>::pointer> {};

template <class Iterator>
inline constexpr bool is_const_iterator_v = is_const_iterator<Iterator>::value;

/* Given a container, view container, and an iterator to a view container,
 * obtains the corresponding iterator to a target container that points
 * to the same element, to which view iterator points to
 */
template <typename Container, typename ViewIterator, typename ViewContainer>
VIEW_API auto get_target_iterator(Container&& c, ViewIterator vit,
                                  ViewContainer&& vc) {
    if constexpr (is_const_iterator_v<ViewIterator>) {
        auto d = std::distance(vc.cbegin(), vit);
        auto it = std::begin(c);
        std::advance(it, d);
        return it;
    } else {
        auto d = std::distance(vc.begin(), vit);
        auto it = std::begin(c);
        std::advance(it, d);
        return it;
    }
}

}  // namespace rviews