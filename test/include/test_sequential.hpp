#include "test.hpp"

/* Set of tests, each accepts an arbinary target container and an
 * arbitary sequential view continaier
 */
template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class> typename Container, typename Type>
    class ViewContainer,
    template <class...> class Container>
void test_sequential_view() {
    {
        ::rviews::cout << "Test vector assign(InputIterator first, "
                          "InputIterator last) via "
                          "vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        DictionaryPage new_data[] = {{5, 'c', {"ccc", "cc"}},
                                     {6, 'd', {"ddd", "dd"}},
                                     {7, 'e', {"eee", "ee"}}};
        ViewContainer view{data};
        view.assign(std::begin(new_data), std::end(new_data));
        assert(ith(data, 0).page == 5);
        assert(ith(data, 1).page == 6);
        assert(ith(data, 2).page == 7);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test vector assign(size_type n, const T& u) via "
                          "vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        view.assign(3, {5, 'c', {"ccc", "cc"}});
        assert(ith(data, 0).page == 5);
        assert(ith(data, 1).page == 5);
        assert(ith(data, 2).page == 5);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector resize(size_type sz) more via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        view.resize(7);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector resize(size_type sz) less via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        view.resize(2);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector resize(size_type sz, const T& c) more via vector "
               "view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        view.resize(6, {0, 'z', {"zzz", "zz"}});
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector resize(size_type sz, const T& c) less via vector "
               "view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        view.resize(2, {0, 'z', {"zzz", "zz"}});
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector push_back(const T& x) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        // lvalue
        DictionaryPage obj{5, 'c', {"ccc", "cc"}};
        view.push_back(obj);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test vector push_back(T&& x) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        // rvalue
        view.push_back({5, 'c', {"ccc", "cc"}});
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test vector pop_back() via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.pop_back();
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector emplace_back(Args&&... args) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.emplace_back(5, 'c', std::vector<std::string>{"ccc", "cc"});
        view.check_consistency();
        assert(ith(data, 4).page == 5);
        assert(ith(view, 4).page == 5);
    }
    {
        ::rviews::cout << "Test vector emplace(const_iterator position, "
                          "Args&&... args) via "
                          "vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.emplace(std::cbegin(view), 0, 'x',
                     std::vector<std::string>{"xxx", "xx"});
        view.emplace(std::cend(view), 5, 'c',
                     std::vector<std::string>{"ccc", "cc"});
        view.check_consistency();
        assert(ith(data, 0).page == 0);
        assert(ith(view, 0).page == 0);
        assert(ith(data, 5).page == 5);
        assert(ith(view, 5).page == 5);
    }
    {
        ::rviews::cout
            << "Test vector insert(const_iterator position, const T& x) via "
               "vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        DictionaryPage obj1{0, 'x', std::vector<std::string>{"xxx", "xx"}};
        DictionaryPage obj2{5, 'c', std::vector<std::string>{"ccc", "cc"}};

        view.insert(std::cbegin(view), obj1);
        view.insert(std::cend(view), obj2);
        view.check_consistency();
        assert(ith(data, 0).page == 0);
        assert(ith(view, 0).page == 0);
        assert(ith(data, 5).page == 5);
        assert(ith(view, 5).page == 5);
    }
    {
        ::rviews::cout
            << "Test vector insert(const_iterator position, T&& x) via vector "
               "view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.insert(std::cbegin(view),
                    {0, 'x', std::vector<std::string>{"xxx", "xx"}});
        view.insert(std::cend(view),
                    {5, 'c', std::vector<std::string>{"ccc", "cc"}});
        view.check_consistency();
        assert(ith(data, 0).page == 0);
        assert(ith(view, 0).page == 0);
        assert(ith(data, 5).page == 5);
        assert(ith(view, 5).page == 5);
    }
    {
        ::rviews::cout << "Test vector insert(const_iterator position, "
                          "size_type n, const T& "
                          "x) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        DictionaryPage obj1{0, 'x', std::vector<std::string>{"xxx", "xx"}};
        DictionaryPage obj2{5, 'c', std::vector<std::string>{"ccc", "cc"}};

        view.insert(std::cbegin(view), 3, obj1);
        view.insert(std::cend(view), 3, obj2);
        view.check_consistency();

        assert(ith(data, 0).page == 0 && ith(data, 1).page == 0 &&
               ith(data, 2).page == 0);
        assert(ith(view, 0).page == 0 && ith(view, 1).page == 0 &&
               ith(view, 2).page == 0);

        assert(ith(data, 7).page == 5 && ith(data, 8).page == 5 &&
               ith(data, 9).page == 5);
        assert(ith(view, 7).page == 5 && ith(view, 8).page == 5 &&
               ith(view, 9).page == 5);
    }
    {
        ::rviews::cout << "Test vector insert(const_iterator position, "
                          "size_type n, const T& "
                          "x) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.insert(std::cbegin(view), std::begin(view) + 1,
                    std::begin(view) + 3);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test vector erase(const_iterator position) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.erase(std::cbegin(view));
        view.check_consistency();
        assert(data.size() == 3);
        assert(view.size() == 3);
        assert(ith(data, 0).page == 2);
        assert(ith(view, 0).page == 2);
    }
    {
        ::rviews::cout << "Test vector erase(const_iterator first, "
                          "const_iterator last) via "
                          "vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.erase(std::cbegin(view) + 1, std::cbegin(view) + 3);
        view.check_consistency();
        assert(data.size() == 2);
        assert(view.size() == 2);
        assert(ith(data, 0).page == 1);
        assert(ith(view, 0).page == 1);
        assert(ith(data, 1).page == 4);
        assert(ith(view, 1).page == 4);
    }
    {
        ::rviews::cout << "Test vector clear() via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};

        view.clear();
        view.check_consistency();
        assert(data.size() == 0);
        assert(view.size() == 0);
    }
    {
        ::rviews::cout
            << "Test vector Type& operator[] (size_type n) via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        DictionaryPage obj = view[0];
        assert(obj.page == ith(data, 0).page);
    }
    {
        ::rviews::cout << "Test vector front via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        DictionaryPage& obj = view.front();
        assert(obj.page == ith(data, 0).page);
    }
    {
        ::rviews::cout << "Test vector back via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        DictionaryPage& obj = view.back();
        assert(obj.page == ith(data, 3).page);
    }
    {
        ::rviews::cout << "Test vector back via vector view: \n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data};
        DictionaryPage& obj = view.back();
        assert(obj.page == ith(data, 3).page);
    }
    // TODO: add tests for const pop and back
}

/* Set of tests, each accepts an arbinary target container and an
 * arbitary sequential view continaier
 */
template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class...> typename Container, typename Type>
    class ViewContainer>
void test_seq_view_on_associate_data() {
    /* TODO: generalize test_algorithms to support containers
    holding a pair of Key, Value instead of just Value and merge
    below tests into them */
    std::map<char, DictionaryPage> data_orig = {
        {'a', {1, 'a', {"aaa", "aab", "abc"}}},
        {'b', {2, 'b', {"bb", "bab", "big"}}},
        {'c', {3, 'c', {"cac", "cb", "ccc"}}},
        {'d', {4, 'd', {"ddd", "de", "dq"}}},
    };

    {
        ::rviews::cout << "Test copy:\n";
        auto data = data_orig;
        ViewContainer view{data};
        std::vector<std::pair<char, DictionaryPage>> out;
        std::copy(std::begin(view), std::end(view), std::back_inserter(out));
        view.check_consistency();
        assert(out.size() == data.size());
        assert(ith(out, 0).second ==
               view[0].second);  // TODO: implement comparison of pairs
    }
    {
        ::rviews::cout << "Test copy_if\n";
        auto data = data_orig;
        ViewContainer view{data};
        std::vector<std::pair<char, DictionaryPage>> out;
        std::copy_if(std::begin(view), std::end(view), std::back_inserter(out),
                     [](auto& elem) { return elem.second.page % 2 == 0; });
        assert(out.size() == 2);
        assert(ith(out, 0).second == view[1].second);
        assert(ith(out, 1).second == view[3].second);
        assert(ith(out, 0).second == data['b']);
        assert(ith(out, 1).second == data['d']);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test copy_n\n";
        auto data = data_orig;
        ViewContainer view{data};
        std::vector<std::pair<char, DictionaryPage>> out;
        std::copy_n(std::begin(view), 3, std::back_inserter(out));
        assert(out.size() == 3);
        assert(ith(out, 0).second == view[0].second);
        assert(ith(out, 1).second == view[1].second);
        assert(ith(out, 2).second == view[2].second);
        view.check_consistency();
    }

    // Here the view container under view object is accessed directly,
    // specifically, its iterators which are pointers. This makes modifications
    // easier due to absence of constness. Doing such operation directly will
    // defintely fail, since what view iterator retursn is Type&. In this case
    // it would be a pair with constant key, so non-modifyable with deleted
    // operators and ctors. But ignoring view iterator and accessing the
    // udnerlying pointers directly will not lead to that.
    /*
    {
        ::rviews::cout << "Test remove\n";
        auto data = data_orig;
        vector_view view{data};
        std::pair<char, DictionaryPage> to_remove{'a', {2, 'a', {"bb", "bab",
    "big"}}}; std::remove_if(std::begin(view._view_container),
    std::end(view._view_container),
            [&to_remove](auto& ptr) {
                return (*ptr).second == to_remove.second;
            }
        );
        view.check_consistency();
    }
    */
}