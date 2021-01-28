#include "test.hpp"

#include "test_algorithms.hpp"
#include "test_associate.hpp"
#include "test_multi.hpp"
#include "test_sequential.hpp"

TEST(map_view, test_namespace_api) {
    std::list<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    ::rviews::map_view view{data, &DictionaryPage::letter};
}
TEST(map_view, nonmodifying_algorithms) {
    nonmodifying_algorithms<map_view, std::vector>();
    nonmodifying_algorithms<map_view, std::list>();
}
TEST(map_view, modifying_algorithms) {
    modifying_algorithms<map_view, std::vector>();
    modifying_algorithms<map_view, std::list>();
}
TEST(map_view, map_on_vector) {
    test_associate_view<map_view, std::vector>();
    test_map_view<map_view, std::vector>();
}
TEST(map_view, map_on_list) {
    test_associate_view<map_view, std::list>();
    test_map_view<map_view, std::list>();
}
TEST(map_view, multimap_on_vector) {
    test_associate_view<multimap_view, std::vector>();
    test_map_view<multimap_view, std::vector>();
}
TEST(map_view, multimap_on_list) {
    test_associate_view<multimap_view, std::list>();
    test_map_view<multimap_view, std::list>();
}

TEST(multimap, common_tests) {
    std::vector<DictionaryPage> dictionary = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'b', {"bb", "bab", "big"}},
        {3, 'c', {"cac", "cb", "ccc"}},
        {4, 'd', {"ddd", "de", "dq"}},
    };
    std::vector<DictionaryPage*> addresses;
    for (auto& e : dictionary) {
        ::rviews::printf("Object at %p\n", &e);
        addresses.push_back(&e);  // store golden address for later testing
        ::rviews::printf("Object page at %p\n", &e.page);
        ::rviews::printf("Object letter at %p\n", &e.letter);
        ::rviews::printf("Object content at %p\n", &e.content);
    }
    ::rviews::printf("Page offset: %lud\n", offsetof(DictionaryPage, page));
    ::rviews::printf("Letter offset: %lud\n", offsetof(DictionaryPage, letter));
    ::rviews::printf("Content offset: %lud\n",
                     offsetof(DictionaryPage, content));
    {
        std::vector<DictionaryPage> multidictionary = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ::rviews::printf("Multimap view by letter:\n");
        multimap_view view{multidictionary, &DictionaryPage::letter};
        view.show_content();
        {
            auto test_range = view.equal_range('a');
            auto it1 = test_range.first;
            auto it2 = test_range.first;
            it2++;
            ::rviews::printf("key = %c\n", it1->first);
            ::rviews::cout << "it1 = " << it1->second << "\n";
            ::rviews::cout << "it2 = " << it2->second << "\n";
            assert(((&multidictionary[0] == &(it2->second)) &&
                    (&multidictionary[1] == &(it1->second))) ||
                   ((&multidictionary[1] == &(it2->second)) &&
                    (&multidictionary[0] == &(it1->second))));
        }
        {
            auto test_range = view.equal_range('b');
            auto it1 = test_range.first;
            auto it2 = test_range.first;
            it2++;
            ::rviews::printf("key = %c\n", it1->first);
            ::rviews::cout << "it1 = " << it1->second << "\n";
            ::rviews::cout << "it2 = " << it2->second << "\n";
            assert(((&multidictionary[2] == &(it2->second)) &&
                    (&multidictionary[3] == &(it1->second))) ||
                   ((&multidictionary[3] == &(it2->second)) &&
                    (&multidictionary[2] == &(it1->second))));
        }
    }
    {
        std::vector<DictionaryPage> multidictionary = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ::rviews::printf("Unordered multimap view by letter:\n");
        unordered_multimap_view view{multidictionary, &DictionaryPage::letter};
        view.show_content();
        {
            auto test_range = view.equal_range('a');
            auto it1 = test_range.first;
            auto it2 = test_range.first;
            it2++;
            ::rviews::printf("key = %c\n", it1->first);
            ::rviews::cout << "it1 = " << it1->second << "\n";
            ::rviews::cout << "it2 = " << it2->second << "\n";
            assert(((&multidictionary[0] == &(it2->second)) &&
                    (&multidictionary[1] == &(it1->second))) ||
                   ((&multidictionary[1] == &(it2->second)) &&
                    (&multidictionary[0] == &(it1->second))));
        }
        {
            auto test_range = view.equal_range('b');
            auto it1 = test_range.first;
            auto it2 = test_range.first;
            it2++;
            ::rviews::printf("key = %c\n", it1->first);
            ::rviews::cout << "it1 = " << it1->second << "\n";
            ::rviews::cout << "it2 = " << it2->second << "\n";
            assert(((&multidictionary[2] == &(it2->second)) &&
                    (&multidictionary[3] == &(it1->second))) ||
                   ((&multidictionary[3] == &(it2->second)) &&
                    (&multidictionary[2] == &(it1->second))));
        }
    }
}
TEST(map_view, iterator_tests) {
    ::rviews::printf("test_iterator started\n");
    std::vector<DictionaryPage> dictionary = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'b', {"bb", "bab", "big"}},
        {3, 'c', {"cac", "cb", "ccc"}},
        {4, 'd', {"ddd", "de", "dq"}},
    };
    {
        ::rviews::printf("Map view by page:\n");
        map_view view{dictionary, &DictionaryPage::letter};
        for (auto& [k, v] : view) {
            ::rviews::cout << "letter = " << k << ", value page = " << v.page
                           << "\n";
        }
    }
    {
        ::rviews::printf("Map view by letter:\n");
        map_view view{dictionary, &DictionaryPage::letter};
        view.show_content();
        for (auto [test, golden] : std::initializer_list<std::pair<char, int>>{
                 {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}}) {
            ::rviews::cout << &view[test] << "\n";
            assert(&view[test] == &dictionary[golden]);
        }
    }
    {
        ::rviews::printf("Map view by page:\n");
        map_view view{dictionary, &DictionaryPage::page};
        view.show_content();
        // Here access by page numbers, not by indexes, its a map, not an array,
        // mind that pages starts from 1 (not from 0) in the original data
        for (auto [test, golden] : std::initializer_list<std::pair<char, int>>{
                 {1, 0}, {2, 1}, {3, 2}, {4, 3}}) {
            ::rviews::cout << &view[test] << "\n";
            assert(&view[test] ==
                   &dictionary[golden]);  // compare address that view sees and
                                          // the golden address
        }
    }
    {
        ::rviews::printf("Multimap view by page:\n");
        multimap_view view{dictionary, &DictionaryPage::letter};
        for (auto [k, v] : view) {
            ::rviews::cout << "letter = " << k << ", value page = " << v.page
                           << "\n";
        }
    }
}
