#pragma once
#include <map>
#include <utility>
#include "associate_view.hpp"

namespace rviews {

template <template <class...> class Container, typename KeyField,
          typename... Types>
class map_view final
    : public associate_view<
          Container<Types...>,
          std::map<KeyField, typename Container<Types...>::value_type&>,
          typename Container<Types...>::value_type, KeyField> {
   public:
    // for template types deduction
    using base = associate_view<
        Container<Types...>,
        std::map<KeyField, typename Container<Types...>::value_type&>,
        typename Container<Types...>::value_type, KeyField>;
    using Type = typename Container<Types...>::value_type;
    map_view(Container<Types...>& container, KeyField Type::*offset)
        : base{container, offset} {}
};

}  // namespace rviews