#include "test.hpp"

#include "test_algorithms.hpp"
#include "test_sequential.hpp"

TEST(vector_tests, test_namespace_api) {
    std::list<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    ::rviews::vector_view view{data};
}

TEST(vector_view, modifying_seq_algorithms) {
    modifying_seq_algorithms<vector_view, std::vector>();
    modifying_seq_algorithms<vector_view, std::list>();
}

TEST(vector_view, sequential_tests) {
    test_sequential_view<vector_view, std::vector>();
    test_sequential_view<vector_view, std::list>();
}

TEST(vector_tests, reserve_more) {
    ::rviews::printf(
        "Test vector reserve(size_type n) more via vector view: \n");
    std::vector<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    vector_view view{data};
    view.reserve(10);
    view.check_consistency();
}
TEST(vector_tests, reserve_less) {
    ::rviews::printf(
        "Test vector reserve(size_type n) less via vector view: \n");
    std::vector<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    vector_view view{data};
    view.reserve(2);
    view.check_consistency();
}
TEST(vector_tests, shrink_to_fit) {
    ::rviews::printf("Test vector shrink_to_fit() via vector view: \n");
    std::vector<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    vector_view view{data};
    view.reserve(10);
    view.shrink_to_fit();
    view.check_consistency();
}
TEST(vector_tests, at) {
    ::rviews::printf("Test vector Type& at(size_type n) via vector view: \n");
    std::vector<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    vector_view view{data};
    DictionaryPage obj = view.at(0);
    assert(obj.page == ith(data, 0).page);
    // TODO: check const operator[] and at
}
TEST(vector_tests, map_experiment) {
    test_seq_view_on_associate_data<vector_view>();
}
TEST(vector_tests, data) {
    ::rviews::printf("Test vector data via vector view: \n");
    std::vector<DictionaryPage> data = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'a', {"bb", "bab", "big"}},
        {3, 'b', {"cac", "cb", "ccc"}},
        {4, 'b', {"ddd", "de", "dq"}},
    };
    vector_view view{data};
    DictionaryPage* raw_data = view.data();
    for (int i : {0, 1, 2, 3}) {
        assert(raw_data[i].page == ith(data, i).page);
    }
    // TODO: add tests for const data()
}

TEST(vector_view, iterator) {
    std::vector<DictionaryPage> dictionary = {
        {1, 'a', {"aaa", "aab", "abc"}},
        {2, 'b', {"bb", "bab", "big"}},
        {3, 'c', {"cac", "cb", "ccc"}},
        {4, 'd', {"ddd", "de", "dq"}},
    };

    for (auto& e : dictionary) {
        ::rviews::printf("Object at %p\n", &e);
        ::rviews::printf("Object page at %p\n", &e.page);
        ::rviews::printf("Object letter at %p\n", &e.letter);
        ::rviews::printf("Object content at %p\n", &e.content);
    }
    ::rviews::printf("Page offset: %lud\n", offsetof(DictionaryPage, page));
    ::rviews::printf("Letter offset: %lud\n", offsetof(DictionaryPage, letter));
    ::rviews::printf("Content offset: %lud\n",
                     offsetof(DictionaryPage, content));

    {
        ::rviews::printf("Vector view:\n");
        vector_view view{dictionary};
        for (auto& e : view) {
            ::rviews::cout << "letter = " << e.letter << ", page = " << e.page
                           << "\n";
        }
    }
    {
        ::rviews::printf("Vector view:\n");
        vector_view view{dictionary};
        view.show_content();
        // Here access by indexes, its an array
        for (auto [test, golden] : std::initializer_list<std::pair<int, int>>{
                 {0, 0}, {1, 1}, {2, 2}, {3, 3}}) {
            ::rviews::cout << &view[test] << "\n";
            assert(&view[test] == &dictionary[golden]);
        }
    }
    {
        ::rviews::printf(
            "Test iterator conversion from target iterators to view "
            "iterators:\n");
        vector_view view{dictionary};

        for (int i : {0, 1, 2, 3}) {
            typename decltype(view)::iterator vit{
                dictionary, std::begin(dictionary) + i, view};
            assert(&(*vit) == &dictionary[i]);
        }
    }
    {
        ::rviews::printf(
            "Test iterator conversion from view iterators to target "
            "iterators:\n");
        vector_view view{dictionary};

        for (int i : {0, 1, 2, 3}) {
            auto vit = std::begin(view);
            std::advance(vit, i);
            typename decltype(dictionary)::iterator it{
                vit.cast_to_target<decltype(dictionary)::iterator>()};
            assert(&(*vit) == &(*it));
        }
    }
}