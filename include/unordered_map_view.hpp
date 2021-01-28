#pragma once
#include <unordered_map>
#include <utility>

#include "associate_view.hpp"

namespace rviews {

template <template <class...> class Container, typename KeyField,
          typename... Types>
class unordered_map_view final
    : public associate_view<
          Container<Types...>,
          std::unordered_map<KeyField,
                             typename Container<Types...>::value_type&>,
          typename Container<Types...>::value_type, KeyField> {
   public:
    using base = associate_view<
        Container<Types...>,
        std::unordered_map<KeyField, typename Container<Types...>::value_type&>,
        typename Container<Types...>::value_type, KeyField>;
    using Type = typename Container<Types...>::value_type;
    unordered_map_view(Container<Types...>& container, KeyField Type::*offset)
        : base{container, offset} {}
};

}  // namespace rviews