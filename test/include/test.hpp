#pragma once

#include <cassert>
#include <string>

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>

#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"

#include "base_view.hpp"
#include "iterator_view.hpp"
#include "list_view.hpp"
#include "map_view.hpp"
#include "multimap_view.hpp"
#include "unordered_map_view.hpp"
#include "unordered_multimap_view.hpp"
#include "vector_view.hpp"

using namespace rviews;

struct DictionaryPage {
   public:
    DictionaryPage() {}
    DictionaryPage(int page_, char letter_, std::vector<std::string> content_)
        : page{page_}, letter{letter_}, content{content_} {
        ::rviews::cout << "ctor ";
        {
            auto& obj = *this;
            ::rviews::cout << &obj << "@{" << obj.page << ", " << obj.letter
                           << ", ";
            for (auto& s : obj.content) {
                ::rviews::cout << s << ", ";
            }
            ::rviews::cout << "}\n";
        }
    }
    bool operator==(const DictionaryPage& other) {
        return (page == other.page) &&
               (letter == other.letter);  // no content cmp
    }
    int page;
    char letter;
    std::vector<std::string> content;
};

__attribute__((unused)) static std::ostream& operator<<(std::ostream& o,
                                                        DictionaryPage& obj) {
    (void)o;
    (void)obj;
    o << &obj << "@{" << obj.page << ", " << obj.letter << ", ";
    for (auto& s : obj.content) {
        o << s << ", ";
    }
    o << "}";
    return o;
}
template <typename Key>
static std::ostream& operator<<(std::ostream& o,
                                std::pair<Key, DictionaryPage>& pair) {
    auto& obj = pair.second;
    o << &obj << "(" << pair.first << ", "
      << "@{" << obj.page << ", " << obj.letter << ", ";
    for (auto& s : obj.content) {
        o << s << ", ";
    }
    o << "})";
    return o;
}

/* Helper function to obtain the i-th element in a container.
 * Note that list does not support [] operator, so this function
 * unifies the access for both list and vector.
 * Return type is not 'auto' since in some tests its not deducible.
 */
template <class Container>
auto& ith(Container& c, int idx) {
    auto it = c.begin();
    std::advance(it, idx);
    return *it;
}