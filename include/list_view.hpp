#pragma once
#include <list>
#include "sequence_view.hpp"

namespace rviews {

template <template <class...> class Container, typename... Types>
class list_view final
    : public sequence_view<Container<Types...>,
                           std::list<typename Container<Types...>::value_type*>,
                           typename Container<Types...>::value_type> {
   public:
    using base =
        sequence_view<Container<Types...>,
                      std::list<typename Container<Types...>::value_type*>,
                      typename Container<Types...>::value_type>;
    list_view(Container<Types...>& container) : base{container} {}
};

}  // namespace rviews