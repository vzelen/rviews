#pragma once
#include <functional>
#include <type_traits>

#include "base_view.hpp"
#include "iterator_view.hpp"

namespace rviews {

template <typename Container, typename ViewContainer, typename Type,
          typename KeyField>
class associate_view {
   public:
    using key_type = typename ViewContainer::key_type;
    using mapped_type = typename ViewContainer::mapped_type;
    using value_type = typename ViewContainer::value_type;
    using size_type = typename ViewContainer::size_type;
    using difference_type = typename ViewContainer::difference_type;
    using reference = typename ViewContainer::reference;
    using const_reference = typename ViewContainer::const_reference;
    using pointer = typename ViewContainer::pointer;
    using const_pointer = typename ViewContainer::const_pointer;
    using node_type = typename ViewContainer::node_type;

    using iterator = associate_iterator_view<typename ViewContainer::iterator>;
    using const_iterator =
        associate_iterator_view<typename ViewContainer::const_iterator>;

    // TODO: check how vit2it, it2vit works when there are duplicates by keys,
    // for example if one of such duplicates is erased

    /* Convert the view iterator that points to a view container
     * to a (target) iterator that points to a view'ed final container */
    auto vit2it(auto& vit) {
        return get_target_iterator(_container, vit, *this);
    }
    /* Convert the normal iterator that points to a normal target container
     * to a view iterator that points to a view container */
    auto it2vit(auto& it) { return _view_container.find((*it).*_offset); }

    /* Obtain an iterator to a view container from a key */
    auto key2vit(const key_type& x) {
        return iterator{_view_container.find(x)};
    }

    VIEW_API iterator begin() { return _view_container.begin(); }
    VIEW_API const_iterator begin() const { return _view_container.begin(); }
    VIEW_API iterator end() { return _view_container.end(); }
    VIEW_API const_iterator end() const { return _view_container.end(); }
    iterator rbegin() { return _view_container.rbegin(); }
    const_iterator rbegin() const { return _view_container.rbegin(); }
    iterator rend() { return _view_container.rend(); }
    const_iterator rend() const { return _view_container.rend(); }
    const_iterator cbegin() { return _view_container.cbegin(); }
    const_iterator cend() { return _view_container.cend(); }
    const_iterator crbegin() const { return _view_container.crbegin(); }
    const_iterator crend() const { return _view_container.crend(); }
    VIEW_API bool empty() const { return _view_container.empty(); }
    VIEW_API size_type size() const { return _view_container.size(); }
    VIEW_API size_type max_size() const { return _view_container.max_size(); }
    // 22.4.4.3, element access
    VIEW_API Type& operator[](const key_type& x) {
        return _view_container.find(x)->second;
    }
    VIEW_API Type& operator[](key_type&& x) {
        return _view_container.find(std::move(x))->second;
    }
    VIEW_API Type& at(const key_type& x) {
        return _view_container.find(x)->second;
    }
    VIEW_API const Type& at(const key_type& x) const {
        return _view_container.find(std::move(x))->second;
    }
    // 22.4.4.4, modifiers
    template <class... Args>
    VIEW_API std::pair<iterator, bool> emplace(Args&&... args) {
        auto it = _container.emplace(std::end(_container), args...);
        populate();
        auto vit = it2vit(it);
        return {vit, true};
    }
    template <class... Args>
    VIEW_API iterator emplace_hint(const_iterator position, Args&&... args) {
        auto it = vit2it(position);
        [[maybe_unused]] auto rit = _container.emplace(it, args...);
        populate();
        auto vit = it2vit(it);
        return vit;
    }
    std::pair<iterator, bool> insert(const value_type& x) {
        auto it = _container.insert(x);
        populate();
        auto vit = it2vit(it);
        return {vit, true};
    }
    std::pair<iterator, bool> insert(value_type&& x) {
        auto it = _container.insert(std::move(x));
        populate();
        auto vit = it2vit(it);
        return {vit, true};
    }
    template <class P>
    std::pair<iterator, bool> insert(P&& x) {
        auto it = _container.insert(std::end(_container), x);
        populate();
        auto vit = it2vit(it);
        return {vit, true};
    }
    iterator insert(const_iterator position, const value_type& x) {
        auto it = vit2it(position);
        auto rit = _container.insert(it, x);
        populate();
        auto vit = it2vit(rit);
        return vit;
    }
    iterator insert(const_iterator position, value_type&& x) {
        auto it = vit2it(position);
        auto rit = _container.insert(it, std::move(x));
        populate();
        auto vit = it2vit(rit);
        return vit;
    }
    template <class P>
    iterator insert(const_iterator position, P&& x) {
        auto it = vit2it(position);
        auto rit = _container.insert(it, std::move(x));
        populate();
        auto vit = it2vit(rit);
        return vit;
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        auto it_first = vit2it(first);
        auto it_last = vit2it(last);
        _container.insert(std::begin(_container), it_first, it_last);
        populate();
    }
    // void insert(initializer_list<value_type>);

    // node_type extract(const_iterator position) {
    //     return _view_container.extract(position);
    // }
    // node_type extract(const key_type& x) {
    //     return _view_container.extract(x);
    // }
    // insert_return_type insert(node_type&& nh) {
    //     return _view_container.insert(std::move(nh));
    // }
    // iterator insert(const_iterator hint, node_type&& nh) {
    //     return _view_container.insert(hint, std::move(nh));
    // }

    // template<class... Args>
    // std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args)
    // {
    //     auto it = _container.emplace(std::end(_container),
    //     std::move(args)...);
    // }
    // template<class... Args>
    // std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
    //     return _view_container.try_emplace(std::move(k), args...);
    // }
    // template<class... Args>
    // iterator try_emplace(const_iterator hint, const key_type& k, Args&&...
    // args) {
    //     return _view_container.try_emplace(hint, k, args...);
    // }
    // template<class... Args>
    // iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args) {
    //     return _view_container.try_emplace(hint, std::move(k), args...);
    // }
    // template<class M>
    // std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) {
    //     return _view_container.insert_or_assign(k, std::move(obj));
    // }
    template <class M>
    std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) {
        auto found = _view_container.find(k);
        if (found == _view_container.end()) {
            auto it = _container.insert(std::end(_container), std::move(obj));
            populate();
            auto vit = it2vit(it);
            return {vit, false};
        } else {
            (*found).second = obj;
            return {found, true};
        }
    }
    // TODO: provide return value properly in insert_or_assign
    template <class M>
    void insert_or_assign([[maybe_unused]] const_iterator hint,
                          const key_type& k, M&& obj) {
        // ignore the hint, since it is useless after the repopulation
        // because hint iterator might become invalid
        insert_or_assign(key_type{k}, std::move(obj));
    }
    template <class M>
    void insert_or_assign([[maybe_unused]] const_iterator hint, key_type&& k,
                          M&& obj) {
        // ignore the hint, since it is useless after the repopulation
        // because hint iterator might become invalid
        insert_or_assign(std::move(k), std::move(obj));
    }
    VIEW_API iterator erase(iterator position) {
        auto it = vit2it(position);
        _container.erase(it);
        populate();
        auto vit = it2vit(it);
        return vit;
    }
    VIEW_API iterator erase(const_iterator position) {
        auto it = vit2it(position);
        _container.erase(it);
        populate();
        auto vit = it2vit(it);
        return vit;
    }
    // TODO: satisfy return type requirement
    VIEW_API void erase(const key_type& x) {
        auto vit = key2vit(x);
        auto it = vit2it(vit);
        _container.erase(it);
        populate();
    }
    VIEW_API iterator erase(const_iterator first, const_iterator last) {
        auto it_first = vit2it(first);
        auto it_last = vit2it(last);
        auto it = _container.erase(it_first, it_last);
        populate();
        auto vit = it2vit(it);
        return vit;
    }
    // void swap(map&)
    // noexcept(allocator_traits<Allocator>::is_always_equal::value &&
    // is_nothrow_swappable_v<Compare>);
    VIEW_API void clear() noexcept {
        _container.clear();
        _view_container.clear();
    }
    // template<class C2>
    // void merge(map<Key, T, C2, Allocator>& source);
    // template<class C2>
    // void merge(map<Key, T, C2, Allocator>&& source);
    // template<class C2>
    // void merge(multimap<Key, T, C2, Allocator>& source);
    // template<class C2>
    // void merge(multimap<Key, T, C2, Allocator>&& source);
    // observers
    // key_compare key_comp() const;
    // value_compare value_comp() const;
    // map operations
    VIEW_API iterator find(const key_type& x) { return key2vit(x); }
    const_iterator find(const key_type& x) const { return key2vit(x); }
    template <class K>
    iterator find(const K& x) {
        return key2vit(x);
    }
    template <class K>
    const_iterator find(const K& x) const {
        return key2vit(x);
    }
    size_type count(const key_type& x) const {
        return _view_container.count(x);
    }
    template <class K>
    size_type count(const K& x) const {
        return _view_container.count(x);
    }
    // C++20:
    // bool contains(const key_type& x) const {
    //     return _view_container.contains(x);
    // }
    // template<class K> bool contains(const K& x) const {
    //     return _view_container.contains(x);
    // }
    iterator lower_bound(const key_type& x) {
        return _view_container.lower_bound(x);
    }
    const_iterator lower_bound(const key_type& x) const {
        return _view_container.lower_bound(x);
    }
    template <class K>
    iterator lower_bound(const K& x) {
        return _view_container.lower_bound(x);
    }
    template <class K>
    const_iterator lower_bound(const K& x) const {
        return _view_container.lower_bound(x);
    }
    iterator upper_bound(const key_type& x) {
        return _view_container.upper_bound(x);
    }
    const_iterator upper_bound(const key_type& x) const {
        return _view_container.upper_bound(x);
    }
    template <class K>
    iterator upper_bound(const K& x) {
        return _view_container.upper_bound(x);
    }
    template <class K>
    const_iterator upper_bound(const K& x) const {
        return _view_container.upper_bound(x);
    }
    std::pair<iterator, iterator> equal_range(const key_type& x) {
        auto [a, b] = _view_container.equal_range(x);
        return {iterator{a}, iterator{b}};
    }
    std::pair<const_iterator, const_iterator> equal_range(
        const key_type& x) const {
        auto [a, b] = _view_container.equal_range(x);
        return {const_iterator{a}, const_iterator{b}};
    }
    template <class K>
    std::pair<iterator, iterator> equal_range(const K& x) {
        auto [a, b] = _view_container.equal_range(x);
        return {iterator{a}, iterator{b}};
    }
    template <class K>
    std::pair<const_iterator, const_iterator> equal_range(const K& x) const {
        auto [a, b] = _view_container.equal_range(x);
        return {const_iterator{a}, const_iterator{b}};
    }
    associate_view(Container& container, KeyField Type::*offset)
        : _container{container}, _offset(offset) {
        populate();
    }
    VIEW_API void show_content() {
        ::rviews::cout << "Continaer content:"
                       << "\n";
        for (auto& e : _container) {
            ::rviews::cout << "Object = " << e << "\n";
        }
        ::rviews::cout << "View continaer content (this):"
                       << "\n";
        for (auto& [k, v] : *this) {
            ::rviews::cout << "Key = " << k << ", value = " << v << "\n";
        }
        ::rviews::cout << "View continaer content (raw):"
                       << "\n";
        for (auto& [k, v] : _view_container) {
            ::rviews::cout << "Key = " << k << ", value = " << v << "\n";
        }
    }
    VIEW_API void check_consistency_by_target() {
        ::rviews::cout << "Target container check: "
                       << "\n";
        for (auto& obj : _container) {
            ::rviews::cout << "Orig " << obj << " ";
            KeyField& k = obj.*_offset;
            auto& view_obj = operator[](k);
            ::rviews::cout << ", view " << view_obj << "\n";
            assert(&obj == &view_obj);
        }
    }
    VIEW_API void check_consistency_by_view() {
        ::rviews::cout << "View container check: "
                       << "\n";
        for (auto ptr = begin(); ptr != end(); ptr++) {
            auto& view_obj = (*ptr).second;
            // ::rviews::cout << "view obj = " << view_obj << "\n";
            auto it = std::find_if(std::begin(_container), std::end(_container),
                                   [&view_obj](auto& obj) {
                                       // ::rviews::cout << "looking at " << obj
                                       //  << " comparing with " << view_obj <<
                                       //  "\n";
                                       return &obj == &(view_obj);
                                   });
            // ::rviews::cout << "container obj = " << *it << "\n";
            if (it != std::end(_container)) {
                auto& obj = *it;
                ::rviews::cout << "Orig " << obj << " "
                               << ", view " << view_obj << "\n";
            } else {
                assert(true);
            }
        }
    }
    VIEW_API void check_consistency() {
        // TODO: print capacity conditionally if it exists
        ::rviews::cout << "Target size = " << _container.size() << "\n";
        ::rviews::cout << "View size = " << _view_container.size() << "\n";
        show_content();
        check_consistency_by_target();
        check_consistency_by_view();
    }

   private:
    VIEW_API void populate() {
        _view_container.clear();
        ::rviews::cout << "populate:: view container cleared"
                       << "\n";
        for (auto& e : _container) {
            ::rviews::cout << "adding src " << e << " into a view by key "
                           << e.*_offset << " => obj in view ";
            _view_container.emplace(e.*_offset, e);
            auto& view_obj = operator[](e.*_offset);
            ::rviews::cout << view_obj << "\n";
        }
    }

    Container& _container;
    KeyField Type::*_offset;
    ViewContainer _view_container;
};

}  // namespace rviews