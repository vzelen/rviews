#pragma once
#include <vector>
#include "sequence_view.hpp"

namespace rviews {

template <template <class...> class Container, typename... Types>
class vector_view final
    : public sequence_view<
          Container<Types...>,
          std::vector<typename Container<Types...>::value_type*>,
          typename Container<Types...>::value_type> {
   public:
    using base =
        sequence_view<Container<Types...>,
                      std::vector<typename Container<Types...>::value_type*>,
                      typename Container<Types...>::value_type>;
    using Type = typename Container<Types...>::value_type;
    vector_view(Container<Types...>& container) : base{container} {}
    constexpr void reserve(typename base::size_type n) {
        base::_container.reserve(n);
        base::_view_container.reserve(n);
        // reallocation possible
        base::populate();
    }
    constexpr Type& at(typename base::size_type n) {
        return *base::_view_container.at(n);
    }
    constexpr const Type& at(typename base::size_type n) const {
        return *base::_view_container.at(n);
    }
    // 22.3.11.4, data access
    constexpr Type* data() noexcept { return base::_container.data(); }
    constexpr const Type* data() const noexcept {
        return base::_container.data();
    }
};

}  // namespace rviews