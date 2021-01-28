#include "test.hpp"

#include "test_algorithms.hpp"
#include "test_sequential.hpp"

TEST(list_tests, test_namespace_api) {
    std::list<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    ::list_view view{data};
}

TEST(list_view, modifying_seq_algorithms) {
    modifying_seq_algorithms<list_view, std::vector>();
    modifying_seq_algorithms<list_view, std::list>();
}

TEST(list_view, sequential_tests) {
    test_sequential_view<list_view, std::vector>();
    test_sequential_view<list_view, std::list>();
}

TEST(list_view, iterator_tests) {
    std::vector<DictionaryPage> dictionary = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'b', {"bb", "bab", "big"}},
        {3, 'c', {"cac", "cb", "ccc"}},
        {4, 'd', {"ddd", "de", "dq"}},
    };
    {
        ::rviews::cout << "List view:\n";
        list_view view{dictionary};
        for (auto& e : view) {
            ::rviews::cout << "letter = " << e.letter << ", page = " << e.page
                           << "\n";
        }
    }
    {
        ::rviews::cout << "List view:\n";
        list_view view{dictionary};
        view.show_content();
        // Here access by list position indexes
        for (auto [test, golden] : std::initializer_list<std::pair<int, int>>{
                 {0, 0}, {1, 1}, {2, 2}, {3, 3}}) {
            ::rviews::cout << &view[test] << "\n";
            assert(&view[test] == &dictionary[golden]);
        }
    }
}
TEST(list_view, map_experiment) {
    test_seq_view_on_associate_data<list_view>();
}