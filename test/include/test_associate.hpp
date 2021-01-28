#include "test.hpp"

template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class> class Container, typename Type,
              typename KeyField> class ViewContainer,
    template <class...> class Container>
void test_associate_view() {
    {
        ::rviews::cout << "Test associate emplace(Args&&... args):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.emplace(5, 'b', std::vector<std::string>{"ddd", "de", "dq"});
        view.check_consistency();
        ::rviews::cout << "&view[4] = " << view[5] << "\n";
        ::rviews::cout << "&data[4] = " << ith(data, 4) << "\n";
        assert(&view[5] == &ith(data, 4));
    }
    {
        ::rviews::cout << "Test associate  emplace_hint(const_iterator "
                          "position, Args&&... "
                          "args):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.emplace_hint(std::cbegin(view), 5, 'b',
                          std::vector<std::string>{"ddd", "de", "dq"});
        view.check_consistency();
        assert(data.size() == 5);
        assert(view.size() == 5);
    }
    {
        ::rviews::cout << "Test associate insert(const value_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        const DictionaryPage obj{5, 'b',
                                 std::vector<std::string>{"ddd", "de", "dq"}};
        view.insert(obj);
        view.check_consistency();
        assert(data.size() == 5);
        assert(view.size() == 5);
    }
    {
        ::rviews::cout << "Test associate insert(const_iterator position, "
                          "const value_type& "
                          "x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        const DictionaryPage obj{5, 'b',
                                 std::vector<std::string>{"ddd", "de", "dq"}};
        view.insert(std::cbegin(view), obj);
        view.check_consistency();
        assert(data.size() == 5);
        assert(view.size() == 5);
    }
    {
        ::rviews::cout
            << "Test associate insert(const_iterator position, value_type&& "
               "x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        const DictionaryPage obj{5, 'b',
                                 std::vector<std::string>{"ddd", "de", "dq"}};
        view.insert(std::cbegin(view), std::move(obj));
        view.check_consistency();
        assert(data.size() == 5);
        assert(view.size() == 5);
    }
    {
        ::rviews::cout
            << "Test associate insert(InputIterator first, InputIterator "
               "last):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.insert(std::begin(view), std::begin(view) + 2);
        view.check_consistency_by_view();
    }
    {
        ::rviews::cout
            << "Test associate insert_or_assign(const key_type& k, M&& obj):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        DictionaryPage obj1{5, 'e', {"eee", "ee"}};
        view.insert_or_assign(5, obj1);
        DictionaryPage obj2{2, 'a', {"aaaaaaa", "aa"}};
        view.insert_or_assign(2, obj2);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test associate insert_or_assign(const_iterator hint, const "
               "key_type& k, M&& obj):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        DictionaryPage obj{5, 'e', {"eee", "ee"}};
        const int key = 5;
        view.insert_or_assign(std::cbegin(view), key, std::move(obj));
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate insert_or_assign(const_iterator "
                          "hint, key_type&& "
                          "k, M&& obj):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        DictionaryPage obj{5, 'e', {"eee", "ee"}};
        view.insert_or_assign(std::cbegin(view), 5, std::move(obj));
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate erase(iterator position):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.erase(std::begin(view));
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate erase(const_iterator position):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.erase(std::cbegin(view));
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate erase(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.erase(3);
        view.check_consistency();
    }
    {
        ::rviews::cout
            << "Test associate erase(const_iterator first, const_iterator "
               "last):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.erase(std::cbegin(view) + 1, std::cbegin(view) + 3);
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate clear():\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.clear();
        view.check_consistency();
    }
    {
        ::rviews::cout << "Test associate find(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        auto vit = view.find(3);
        auto& obj = ith(data, 2);
        auto& vobj = (*vit).second;
        assert(&obj == &vobj);
        view.check_consistency();
    }
    // TODO: check constness of find
    //      const_iterator find(const key_type& x)
    //      template<class K> iterator find(const K& x)
    //      template<class K> const_iterator find(const K& x)
    {
        ::rviews::cout << "Test associate count(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        assert(view.count(1) == 1);
        assert(view.count(2) == 1);
        assert(view.count(3) == 1);
        assert(view.count(4) == 1);
        assert(view.count(5) == 0);
        view.check_consistency();
    }
    // TODO: check template<class K> size_type count(const K& x) const
    // C++20:
    // {
    //     ::rviews::cout << "Test associate contains(const key_type& x):\n";
    //     Container<DictionaryPage> data = {
    //         {1, 'a', {"aaa", "aab", "abc"}},
    //         {2, 'a', {"bb", "bab", "big"}},
    //         {3, 'b', {"cac", "cb", "ccc"}},
    //         {4, 'b', {"ddd", "de", "dq"}},
    //     };
    //     ViewContainer view{data, &DictionaryPage::page};
    //     assert(view.contains(1) == true);
    //     assert(view.contains(2) == true);
    //     assert(view.contains(3) == true);
    //     assert(view.contains(4) == true);
    //     assert(view.contains(5) == false);
    //     view.check_consistency();
    // }
    {
        ::rviews::cout << "Test associate equal_range(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        view.check_consistency();
        auto range = view.equal_range(2);
        for (auto it = range.first; it != range.second; it++) {
            auto& obj = (*it).second;
            ::rviews::cout << obj << "\n";
        }
        // should be 1 element in the range which equals to 1st in data
        assert(&(*(range.first)).second == &ith(data, 1));
    }
    // TODO: std::pair<const_iterator, const_iterator> equal_range(const
    // key_type& x) const
    //       std::pair<iterator, iterator> equal_range(const K& x)
    //       std::pair<const_iterator, const_iterator> equal_range(const K& x)
    //       const
}

template <
    // ViewContainer - a view container, e.g. vector_view, list_view, etc
    // Container - a target container, e.g. std::vector, std::list
    template <template <class> class Container, typename Type,
              typename KeyField> class ViewContainer,
    template <class...> class Container>
void test_map_view() {
    {
        ::rviews::cout << "Test associate lower_bound(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        {
            auto vit = view.lower_bound(0);
            auto& vobj = (*vit).second;
            ::rviews::cout << "lower bound (0) == " << vobj << "\n";
            assert(&vobj == &ith(data, 0));
        }
        {
            auto vit = view.lower_bound(10);
            assert(vit == std::end(view));
        }
        view.check_consistency();
    }
    // TODO: check const_iterator upper_bound(const key_type& x) const
    //       template<class K> iterator upper_bound(const K& x)
    //       template<class K> const_iterator upper_bound(const K& x) const
    {
        ::rviews::cout << "Test associate upper_bound(const key_type& x):\n";
        Container<DictionaryPage> data = {
            {1, 'a', {"aaa", "aab", "abc"}},
            {2, 'a', {"bb", "bab", "big"}},
            {3, 'b', {"cac", "cb", "ccc"}},
            {4, 'b', {"ddd", "de", "dq"}},
        };
        ViewContainer view{data, &DictionaryPage::page};
        {
            auto vit = view.upper_bound(0);
            auto& vobj = (*vit).second;
            ::rviews::cout << "upper bound (0) == " << vobj << "\n";
            assert(&vobj == &ith(data, 0));
        }
        {
            auto vit = view.upper_bound(10);
            assert(vit == std::end(view));
        }
        view.check_consistency();
    }
}
