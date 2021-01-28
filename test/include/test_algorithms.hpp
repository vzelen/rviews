#include "test.hpp"

template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class> class Container, typename Type,
              typename KeyField> class ViewContainer,
    template <class...> class Container>
void nonmodifying_algorithms() {
    {
        ::rviews::cout << "Test associate for_each:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        std::for_each(std::begin(view), std::end(view), [](auto& vobj) {
            ::rviews::cout << vobj.second << "\n";
        });
    }
    {
        ::rviews::cout << "Test associate count:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        std::pair<const int, DictionaryPage&> target{1, ith(data, 0)};
        auto n_equal = std::count(std::begin(view), std::end(view), target);
        view.check_consistency();
        assert(n_equal == 1);
    }
    {
        ::rviews::cout << "Test associate count_if:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        auto n_equal = std::count_if(
            std::begin(view), std::end(view),
            [&data](auto& vobj) { return ith(data, 0) == vobj.second; });
        view.check_consistency();
        assert(n_equal == 1);
    }
    {
        ::rviews::cout << "Test associate find:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        std::pair<const int, DictionaryPage&> target{1, ith(data, 0)};
        auto found = std::find(std::begin(view), std::end(view), target);
        view.check_consistency();
        assert(&found->second == &ith(data, 0));
    }
    {
        ::rviews::cout << "Test associate find_if:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        std::pair<const int, DictionaryPage&> target{1, ith(data, 0)};
        auto found = std::find_if(
            std::begin(view), std::end(view),
            [&data](auto& vobj) { return &vobj.second == &ith(data, 0); });
        view.check_consistency();
        assert(&found->second == &ith(data, 0));
    }
}

template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class> class Container, typename Type,
              typename KeyField> class ViewContainer,
    template <class...> class Container>
void modifying_algorithms() {
    // std::copy, move, fill, generate, remove, replace, etc use deleted
    // operator= for std pair containing a reference to T
    {
        ::rviews::cout << "Test associate transform:\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        std::vector<DictionaryPage> output;
        std::transform(std::begin(view), std::end(view),
                       std::back_inserter(output),
                       [](auto& vobj) { return vobj.second; });
        std::vector<std::pair<const int, DictionaryPage&>> output2;
    }
}

template <template <class...> class Container, typename DictionaryPage,
          bool is_pair>
constexpr auto prepare_input_data() {
    if constexpr (is_pair) {
        Container<int, DictionaryPage> data = {
            {1, {1, 'a', {"aaa", "aab", "abc"}}},
            {2, {2, 'b', {"bb", "bab", "big"}}},
            {3, {3, 'c', {"cac", "cb", "ccc"}}},
            {4, {4, 'd', {"ddd", "de", "dq"}}},
        };
        return data;
    } else {
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        return data;
    }
}

template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class...> class Container, typename Type>
    class ViewContainer,
    template <class...> class Container, bool is_pair = false>
void modifying_seq_algorithms() {
    {
        ::rviews::cout << "Test copy:\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        Container<DictionaryPage> out;
        std::copy(std::begin(view), std::end(view), std::back_inserter(out));
        view.check_consistency();
        assert(out.size() == data.size());
        assert(ith(out, 0) == view[0]);
    }
    {
        ::rviews::cout << "Test copy_if\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        Container<DictionaryPage> out;
        std::copy_if(std::begin(view), std::end(view), std::back_inserter(out),
                     [](auto& elem) { return elem.page % 2 == 0; });
        assert(out.size() == 2);
        assert(ith(out, 0) == view[1] /* == 2 */);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test copy_n\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        Container<DictionaryPage> out;
        std::copy_n(std::begin(view), 3, std::back_inserter(out));
        assert(out.size() == 3);
        assert(ith(out, 0) == view[0]);
        assert(ith(out, 1) == view[1]);
        assert(ith(out, 2) == view[2]);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test fill\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        DictionaryPage filler{5, 'e',
                              std::vector<std::string>{"eee", "ee", "e"}};
        std::fill(std::begin(view), std::end(view), filler);
        assert(view.size() == 4);
        for (auto&& e : view) {
            assert(e == filler);
        }
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test remove\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        DictionaryPage to_remove{2, 'a', {"bb", "bab", "big"}};
        std::remove(std::begin(view), std::end(view), to_remove);
        view.check_consistency();
        assert(view[0].page == 1);
        assert(view[1].page == 3);
        assert(view[2].page == 4);
    }
    {
        ::rviews::cout << "Test remove_if\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        std::remove_if(std::begin(view), std::end(view),
                       [](auto& obj) { return obj.page == 2; });
        view.check_consistency();
        assert(view[0].page == 1);
        assert(view[1].page == 3);
        assert(view[2].page == 4);
    }
    {
        ::rviews::cout << "Test generate\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        std::generate(std::begin(view), std::end(view), []() -> DictionaryPage {
            return {5, 'e', {"eee", "ee", "e"}};
        });
        view.check_consistency();
        for (auto& e : view) {
            assert(e.page == 5);
        }
    }
    {
        ::rviews::cout << "Test generate_n\n";
        auto data = prepare_input_data<Container, DictionaryPage, is_pair>();
        ViewContainer view{data};
        std::generate_n(std::begin(view), 2, []() -> DictionaryPage {
            return {5, 'e', {"eee", "ee", "e"}};
        });
        view.check_consistency();
        assert(view[0].page == 5);
        assert(view[1].page == 5);
        assert(view[2].page == 3);
        assert(view[3].page == 4);
    }
}
