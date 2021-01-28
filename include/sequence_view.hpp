#pragma once
#include <algorithm>
#include <list>

#ifdef VIEW_TESTS
#include <cassert>
#endif

#include "iterator_view.hpp"

namespace rviews {

template <typename Container, typename ViewContainer, typename Type>
class sequence_view {
   public:
    using T = Type;
    using value_type = typename ViewContainer::value_type;
    using allocator_type = typename ViewContainer::allocator_type;
    using pointer = typename ViewContainer::pointer;
    using const_pointer = typename ViewContainer::const_pointer;
    using reference = Type&;
    using const_reference = const Type&;
    using size_type = typename ViewContainer::size_type;
    using difference_type = typename ViewContainer::difference_type;
    using reverse_iterator = typename ViewContainer::reverse_iterator;
    using const_reverse_iterator =
        typename ViewContainer::const_reverse_iterator;

    using iterator = sequential_iterator_view<typename ViewContainer::iterator>;
    using const_iterator =
        sequential_iterator_view<typename ViewContainer::const_iterator>;

    // 22.3.11.2, construct/copy/destroy
    // constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator())
    // { } constexpr explicit vector(const Allocator&) noexcept; constexpr
    // explicit vector(size_type n, const Allocator& = Allocator()); constexpr
    // vector(size_type n, const T& value, const Allocator& = Allocator());
    // template<class InputIterator>
    // constexpr vector(InputIterator first, InputIterator last, const
    // Allocator& = Allocator()); constexpr vector(const vector& x); constexpr
    // vector(vector&&) noexcept; constexpr vector(const vector&, const
    // Allocator&); constexpr vector(vector&&, const Allocator&); constexpr
    // vector(initializer_list<T>, const Allocator& = Allocator()); constexpr
    // ~vector(); constexpr vector& operator=(const vector& x);

    // constexpr vector& operator=(vector&& x)
    // noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    // || allocator_traits<Allocator>::is_always_equal::value);

    // constexpr vector& operator=(initializer_list<T>);

    template <class InputIterator>
    constexpr void assign(InputIterator first, InputIterator last) {
        _container.assign(first, last);
        populate();
    }
    constexpr void assign(size_type n, const T& u) {
        _container.assign(n, u);
        populate();
    }
    // constexpr void assign(initializer_list<T>);
    // constexpr allocator_type get_allocator() const noexcept;
    // iterators
    constexpr iterator explicit_begin() noexcept {
        return _view_container.begin();
    }
    constexpr iterator begin() noexcept { return _view_container.begin(); }
    constexpr const_iterator begin() const noexcept {
        return _view_container.begin();
    }
    constexpr iterator end() noexcept { return _view_container.end(); }
    constexpr const_iterator end() const noexcept {
        return _view_container.end();
    }
    constexpr reverse_iterator rbegin() noexcept {
        return _view_container.rbegin();
    }
    constexpr const_reverse_iterator rbegin() const noexcept {
        return _view_container.rbegin();
    }
    constexpr reverse_iterator rend() noexcept {
        return _view_container.rend();
    }
    constexpr const_reverse_iterator rend() const noexcept {
        return _view_container.rend();
    }
    constexpr const_iterator cbegin() const noexcept {
        return _view_container.cbegin();
    }
    constexpr const_iterator cend() const noexcept {
        return _view_container.cend();
    }
    constexpr const_reverse_iterator crbegin() const noexcept {
        return _view_container.crbegin();
    }
    constexpr const_reverse_iterator crend() const noexcept {
        return _view_container.crend();
    }
    // 22.3.11.3, capacity
    [[nodiscard]] constexpr bool empty() const noexcept {
        return _view_container.empty();
    }
    constexpr size_type size() const noexcept { return _view_container.size(); }
    constexpr size_type max_size() const noexcept {
        return _view_container.max_size();
    }
    constexpr size_type capacity() const noexcept {
        return _view_container.capacity();
    }
    constexpr void resize(size_type sz) {
        auto orig_sz = _container.size();
        _container.resize(sz);
        _view_container.resize(sz);
        if (sz > orig_sz) {
            populate();
        }
    }
    constexpr void resize(size_type sz, const T& c) {
        _container.resize(sz, c);
        // reallocation possible
        populate();
    }
    // constexpr void reserve(size_type n) {
    //     _container.reserve(n);
    //     _view_container.reserve(n);
    //     // reallocation possible
    //     populate();
    // }
    constexpr void shrink_to_fit() {
        _container.shrink_to_fit();
        _view_container.shrink_to_fit();
        // reallocation possible
        populate();
    }
    // element access
    constexpr Type& operator[](size_type n) {
        iterator it = std::begin(_view_container);
        std::advance(it, n);
        return *it;
    }
    constexpr const Type& operator[](size_type n) const {
        iterator it = std::begin(_view_container);
        std::advance(it, n);
        return *it;
    }

    // Below _view_container iterators are casted to iterators
    constexpr reference front() { return *_view_container.front(); }
    constexpr const_reference front() const { return *_view_container.front(); }
    constexpr reference back() { return *_view_container.back(); }
    constexpr const_reference back() const { return *_view_container.back(); }
    // 22.3.11.5, modifiers
    template <class... Args>
    constexpr Type& emplace_back(Args&&... args) {
        auto vit = emplace(cend(), std::move(args)...);
        return *vit;
    }
    constexpr void push_back(const T& x) {
        _container.push_back(x);
        // reallocation might takes place
        populate();
    }
    constexpr void push_back(T&& x) {
        _container.push_back(std::move(x));
        // reallocation might takes place
        populate();
    }
    constexpr void pop_back() {
        _container.pop_back();
        populate();
    }
    template <class... Args>
    constexpr iterator emplace(const_iterator position, Args&&... args) {
        auto it = get_target_iterator(_container, position, *this);
        auto ret = _container.emplace(it, std::move(args)...);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    constexpr iterator insert(const_iterator position, const T& x) {
        auto it = get_target_iterator(_container, position, *this);
        auto ret = _container.insert(it, x);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    constexpr iterator insert(const_iterator position, T&& x) {
        auto it = get_target_iterator(_container, position, *this);
        auto ret = _container.insert(it, std::move(x));
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    constexpr iterator insert(const_iterator position, size_type n,
                              const T& x) {
        auto it = get_target_iterator(_container, position, *this);
        auto ret = _container.insert(it, n, x);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }

    template <class InputIterator>
    constexpr iterator insert(const_iterator position, InputIterator first,
                              InputIterator last) {
        auto it = get_target_iterator(_container, position, *this);
        auto it_first = get_target_iterator(_container, first, *this);
        auto it_last = get_target_iterator(_container, last, *this);
        auto ret = _container.insert(it, it_first, it_last);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    // constexpr iterator insert(const_iterator position, initializer_list<T>
    // il);
    constexpr iterator erase(const_iterator position) {
        auto it = get_target_iterator(_container, position, *this);
        auto ret = _container.erase(it);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    constexpr iterator erase(const_iterator first, const_iterator last) {
        auto it_first = get_target_iterator(_container, first, *this);
        auto it_last = get_target_iterator(_container, last, *this);
        auto ret = _container.erase(it_first, it_last);
        populate();
        iterator vit{_container, ret, _view_container};
        return vit;
    }
    // constexpr void swap(vector&)
    // noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value
    // || allocator_traits<Allocator>::is_always_equal::value);
    constexpr void clear() noexcept {
        _container.clear();
        _view_container.clear();
    }
    sequence_view(Container& container) : _container{container} {
        // populate the content of internal container with pointers to target
        // objects
        populate();
    }
    void show_content() {
        ::rviews::cout << "Content:\n";
        for (unsigned int i = 0; i < _container.size(); i++) {
            iterator it = _view_container.begin();
            std::advance(it, i);
            Type& x = *it;
            ::rviews::cout << "Orig " << &_container[i] << " "
                           << ", view " << x << "\n";
        }
    }
    /* Check if the view and view'ed container are consistent, e.g.
     * everything what view sees still exists, and everything what exists
     * in target container is still viewable by the view
     */
    void check_consistency() {
        // TODO: print capacity conditionally if it exists
        ::rviews::cout << "Target size = " << _container.size() << "\n";
        ::rviews::cout << "View size = " << _view_container.size() << "\n";
        ::rviews::cout << "Target container check: "
                       << "\n";
        for (unsigned int i = 0; i < _container.size(); i++) {
            auto it = std::begin(_container);
            std::advance(it, i);
            iterator view_it = std::begin(_view_container);
            std::advance(view_it, i);
            ::rviews::cout << "Orig " << *it << " "
                           << ", view " << *view_it << "\n";
            assert(&(*it) == &(*view_it));
        }
        ::rviews::cout << "View container check: "
                       << "\n";
        for (unsigned int i = 0; i < _view_container.size(); i++) {
            auto it = std::begin(_container);
            assert(i < _container.size());  // check if memory is valid
            std::advance(it, i);
            iterator view_it = std::begin(_view_container);
            std::advance(view_it, i);
            ::rviews::cout << "Orig " << *it << " "
                           << ", view " << *view_it << "\n";
            assert(&(*it) == &(*view_it));
        }
    }

   protected:
    void populate() {
        _view_container.clear();
        std::transform(std::begin(_container), std::end(_container),
                       std::back_inserter(_view_container),
                       [](Type& t) { return &t; });
    }
    Container& _container;
    ViewContainer _view_container;
};

}  // namespace rviews