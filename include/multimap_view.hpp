#pragma once
#include <map>

#include "associate_view.hpp"

namespace rviews {

template <template <class...> class Container, typename KeyField,
          typename... Types>
class multimap_view final
    : public associate_view<
          Container<Types...>,
          std::multimap<KeyField, typename Container<Types...>::value_type&>,
          typename Container<Types...>::value_type, KeyField> {
   public:
    using base = associate_view<
        Container<Types...>,
        std::multimap<KeyField, typename Container<Types...>::value_type&>,
        typename Container<Types...>::value_type, KeyField>;
    using Type = typename Container<Types...>::value_type;
    multimap_view(Container<Types...>& container, KeyField Type::*offset)
        : base{container, offset} {}
};

}  // namespace rviews