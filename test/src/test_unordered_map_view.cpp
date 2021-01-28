#include "test.hpp"

#include "test_algorithms.hpp"
#include "test_associate.hpp"
#include "test_multi.hpp"
#include "test_sequential.hpp"
TEST(unordered_map_view, test_namespace_api) {
    std::list<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    ::rviews::unordered_map_view view{data, &DictionaryPage::letter};
}
TEST(unordered_map_view, nonmodifying_algorithms) {
    nonmodifying_algorithms<unordered_map_view, std::vector>();
    nonmodifying_algorithms<unordered_map_view, std::list>();
}
TEST(unordered_map_view, modifying_algorithms) {
    modifying_algorithms<unordered_map_view, std::vector>();
    modifying_algorithms<unordered_map_view, std::list>();
}
TEST(unordered_map_view, unordered_map_on_vector) {
    test_associate_view<unordered_map_view, std::vector>();
}
TEST(unordered_map_view, unordered_map_on_list) {
    test_associate_view<unordered_map_view, std::list>();
}
TEST(unordered_map_view, unordered_multimap_on_vector) {
    test_associate_view<unordered_multimap_view, std::vector>();
}
TEST(unordered_map_view, unordered_multimap_on_list) {
    test_associate_view<unordered_multimap_view, std::list>();
}
TEST(unordered_map_view, iterator_tests) {
    std::vector<DictionaryPage> dictionary = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'b', {"bb", "bab", "big"}},
        {3, 'c', {"cac", "cb", "ccc"}},
        {4, 'd', {"ddd", "de", "dq"}},
    };
    {
        ::rviews::cout << "Unordered map view by page:\n";
        unordered_map_view view{dictionary, &DictionaryPage::letter};
        for (auto [k, v] : view) {
            ::rviews::cout << "letter = " << k << ", value page = " << v.page
                           << "\n";
        }
    }
    {
        ::rviews::cout << "Unordered multimap view by page:\n";
        unordered_map_view view{dictionary, &DictionaryPage::letter};
        for (auto [k, v] : view) {
            ::rviews::cout << "letter = " << k << ", value page = " << v.page
                           << "\n";
        }
    }
    {
        ::rviews::cout << "Unordered map view by letter:\n";
        unordered_map_view view{dictionary, &DictionaryPage::letter};
        view.show_content();
        for (auto [test, golden] : std::initializer_list<std::pair<char, int>>{
                 {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}}) {
            ::rviews::cout << &view[test] << "\n";
            assert(&view[test] == &dictionary[golden]);
        }
    }
}