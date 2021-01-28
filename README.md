# Reference Views - C++ header-only library
[![Language](https://img.shields.io/badge/C%2B%2B-17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![Actions Status](https://github.com/pleaded/rviews/workflows/Ubuntu/badge.svg)](https://github.com/pleaded/rviews/workflows/Ubuntu/badge.svg)

```
                       +---+        
             x---------| B*|---------x      
Map          |         +---+         |      
View:      +---+         |         +---+    
           | A*|         |         | C*|    
           +---+         |         +---+    
             |           |           |  
Original   +---+       +---+       +---+
List:      | A |------>| B |------>| C |
           +---+       +---+       +---+
             |           |           |       
Vector     +--------x---------x--------+    
View:      |   A*   |    B*   |    C*  |    
           +--------x---------x--------+    
```
rviews (reference views) is a header-only C++17 library which allows to view an STL container via another STL container with lvalue semantics. E.g. you could view a list as it would be a vector, or even as it would be a map,or even both.

```c++
std::list<char> data{'a', 'b', 'c'};
vector_view view{data};
std::cout << view[0] << std::endl; // a
std::cout << view[1] << std::endl; // b
std::cout << view[2] << std::endl; // c
```

# Overview

All possible combinations that you could get on top of STL containers:

| Source \ View        | view as `array` |view as `vector` | view as `list` | view as `map` | view as `unordered_map` | view as `set` | view as `multimap` | view as `unordered_multimap` |
| - |:---:| :------:|:----:|:---:|:-------------:|:---:|:---:|:---:|
| source is `array`||✅|✅|✅|✅| |✅|✅|
| source is `vector`||✅|✅|✅|✅| |✅|✅|
| source is `list`||✅|✅|✅|✅| |✅|✅|
| source is `map`||✅|✅||||||
| source is `unordered_map`|||||||||
| source is `set`|||||||||
| source is `multimap`|||||||||
| source is `unordered_multimap`|||||||||

Please respect the `const`ness of keys in associate containers, all qualifiers are propagated to view containers. So if you view a map as a vector of pairs, it would be not possible to modify the first element of a pair (since it represents a key and holds `const` qualifier).

The usecases where you might want to use it:

- there is a sequentially stored collection of components of type `T`, stored in `vector` or `list`, but semantically the type `T` has a "key"
which would be good to use for an associative access. You could store the elements in a `map` or `unordered_map` instead in first place, but then you will loose
the properties of sequential storage. Alternatively, there might be already an established C++ infrastructure where it is not possible
to easily change the underlying container types from a `vector` to a `map`. What you could do naturally - you could make an associate container and store pointer manually to original elements from a
`vector`/`list`, but then you will always have to dereference the pointers. Moreover, you might want to add / remove elements via an associate view,
and it would be beneficial in case the underlying sequential containers updates automatically. E.g. you have to manually keep the underlying sequential container of objects and the associate container of pointers to the objects in sync. So this is exactly what `rviews` library does, but also with reference semantics

# Features

* support of an associative API (e.g. `map`) for sequentially stored data (e.g. `vector`)

```c++
struct Complex {
    int id;
    char ch;
}

std::vector<Complex> data{{1, 'a'}, {2, 'b'}, {3, 'c'}};
map_view view{data, &Complex::ch};

std::cout << view['a'].id << std::endl; // 1
std::cout << view['b'].id << std::endl; // 2
std::cout << view['c'].id << std::endl; // 3
```


* **reference semantics** for the view'ed elements
```c++
vector_view as_vector{container};
f(as_vector[0].id); // not f(as_vector[0]->id) or f(as_vector[0].get().id)
```
* **direct lvalue access** for view'ed elements
```c++
vector_view view{container};
map_view view{container, &Complex::ch};

auto& obj = as_vector[0]; // not limited to auto obj = as_vector[0];
auto& [key, value] = as_map['a']; // not limited to auto [key, value] = as_map['a'];
```

# Usage
This is a header-only library. So just include it:
```c++
#include "vector_view.hpp"
```

|Source file|View Class|
|-----------|----------|
|`vector_view.hpp`|`rviews::vector_view(SourceContainer<T>)`|
|`list_view.hpp`|`rviews::list_view(SourceContainer<T>)`|
|`map_view.hpp`|`rviews::map_view(SourceContainer<T>, Key)`|
|`multimap_view.hpp`|`rviews::multimap_view(SourceContainer<T>, Key)`|
|`unordered_map_view.hpp`|`rviews::unordered_map_view(SourceContainer<T>, Key)`|
|`unordered_multimap_view.hpp`|`rviews::unordered_multimap_view(SourceContainer<T>, Key)`|

Each view accepts the source container (which will be viewed). Associate container accepts the key which is obtainable from the elements stored inside the source container in a form of a pointer to a member data field of `T` stored inside source container.

Please find `example` folder. It contains a separate cmake project with `CMakeLists.txt` and shows how to include and use `rviews` library.

```
target_link_libraries(your_project_name PUBLIC rviews)
```

Note: the library in the development, so consider that during the usage. Most likely the views API remains stable, but there are a lot of underlying optimization which might be done under the hood, for example the ability of a view container to detect source container's relocations and do not recreate the internal view state after each change so agressively (instead, only when the invalidation of pointers really takes place).

# Tests & Code Coverage
Tests are covering the usage of STL algorithms and STL container API itself on the viewed objects.
GTest framework is used and assumed to be available in the system. 

Building & running tests:
```bash
$ make test
```

Building & running tests & providing code coverage report in html (under coverage/index.html):
```bash
$ make coverage # it uses gcovr, so install it
```

# TODO
- [ ] `array` view
- [ ] `set` view
- [ ] `unordered_map` as a source
- [ ] improve the performance of modification of source containers via a view container by reducing the amount of recreations of view container when source container does not reallocate
- [ ] reduce the amount of view container repopulation, almost always it is not needed 
- [ ] improved tests for `map` as a source
- [ ] `extract` for associate containers
- [ ] `insert(const_iterator hint, node_type&& nh)` for associate containers
- [ ] `try_emplace`
- [ ] `insert_or_assign` - assigning value to a key which does not mathc with the value (e.g. obj has key = 5, while assinging this object to key = 2)
- [ ] perfect forward more
- [ ] add the syncronization between multiple views
- [ ] different TODOs from the code, just grep `TODO`
- [ ] add windows & macos builds in CI

# Implementation details

Under the hood a view containers stores pointer-like-types (`T*` or `T&`) to the original objects of target container:

| View Container | Stored object type | Internal storage |
| ----------| -------| ------------|
| `vector_view` | `T*` | `std::vector` |
| `list_view` | `T*` | `std::list` |
| `map_view` | `T&` | `std::map` |
| `multimap_view` | `T&` | `std::multimap` |
| `unordered_map_view` | `T&` | `std::unordered_map` |
| `unordered_multimap_view` | `T&` | `std::unordered_multimap` |

E.g. `vector_view` stores raw pointers to original target containers, and `map_view` stores references to the objects of original target container.

Each view container interanlly has a container of view type, e.g. `vector_view` has `std::vector` inside, and `map_view` has `std::map` inside, etc. Thus, the view library entirely relies on the STL implementation.

Why some view containers (mostly, associate ones) stores references (`T&`) instead of raw pointers (`T*`)? Because the underlying stored type of those is `std::pair<const KeyType, T>` and we don't have a syntax to obtain lvalue reference from a pointer if it's placed inside an aggregate type. 

Consider the following example:
```c++
template<typename T>
struct Wrapper {
    using ptr_type = T*;
    using ref_type = T&;
    ptr_type ptr;
    ref_type get() {
        return *ptr; // ok
    }
};

struct UserData {
    int a;
    char b;
};

void foo() {
    UserData user{42, 'a'};
    Wrapper wrapper{&user};
    UserData& ref = wrapper.get();
}
```
This works fine, we store a pointer `int * ptr` inside `Wrapper` object, and provide an lvalue reference in `get()` method.

Now assume our function `get()` returns not just an lvalue reference but an additional return parameter (let's say a `char`). We don't have in-language support for functions with multuple return parameters, so we either return a `struct` or `pair`. For the sake of example, lets return a `pair`. Keep your eyes on what just happened: we had a function `get()` returning one parameter, now we _just_ return something _additionally_, sounds conceptually simple, isn't it?

```c++
template<typename T>
struct Wrapper {
    using ptr_type = std::pair<char, T*>;
    using ref_type = std::pair<char, T&>;
    ptr_type ptr;
    ref_type get() {
        return {ptr.first, *ptr.second};
    }
};

struct UserData {
    int a;
    char b;
};

void foo() {
    UserData user{42, 'a'};
    Wrapper wrapper{&user};
    auto& [key, ref] = wrapper.get(); // error
    /* cannot bind non-const lvalue reference 
     of type 'std::pair<char, int&>&' to an rvalue
     of type 'Wrapper2::ptr_ref' {aka 'std::pair<char, int&>'} */
}
```

We store a pointer? We can "dereference" it to get lvalue.
We store a pair of something and a pointer? We can't return a pair of something and "dereferenced lvalue".

Formally speaking: because `std::pair<A, B*>` and `std::pair<A, B&>` are different types. 
Informally speaking: we can create `int&` from `int*` by `*ptr` syntax, and we cant create `something + int&` from `something + int*`? Because we don't have this `+` and ability to return multiple values.

Even worse, de-facto `std::pair<something, T*>` and `std::pair<something, T&>` have same memory layout (but this is dangerous information which should be ignored) - with a pointer in `second` in both cases. But we cant "cast" one type into another (even if they store exactly same bytes on low-level), any kind of `reinterpret_cast` would be UB here.

Fundamentally speaking, the answer for such situation is "just don't do that, look, you have 2 different types, so distinguish between them explicitly". Yes, and no.

Can we provide an implcit access syntax for a user if we store just a pointer and nothing more? Yes, user can do this:
```c++
T& obj = wrapper.get();
f(obj.field);
// ...
```

Can we achive the same in case we store _a bit more_ information than just a pointer, for example we store a key in a `std::map` iterator which might be, for example, a `char`?
```c++
auto& [key, obj] = wrapper.get();
f(obj.field);
```
No, this is not possible, because we did `auto&` instead of `auto`. That means that we "referencing" to some "existing lvalue in memory". But `wrapper` stores a pointer, while user API requires an explicit access to an object (`f(obj.field)`, and not `f(obj->field)`, or `f(obj.get().field)`, or any kind of other proxy middleware). So `get()` needs to "dereference" to get lvalue from a stored pointer. All good, this is how it worked with previous "original" example above without additional `char` keys. But now we have a `char` key next to a pointer and need a kind of a pair. This mandates us to create a _new_ object. Because the process of (1) obtaining an lvalue `T&` from `T*` by `*ptr` syntax and the process of (2) returning two parameters from a function - those 2 processes can't be combined.

Of course a user could do that:
```c++
auto tmp = wrapper.get();
auto key = tmp.first;
auto& obj = tmp.second;
f(obj.field);
```
e.g. obtain the pair clasically by value, then bind `first` by-value and `second` by-ref. This is an approach to consider `first` as a very lightweigt type which can be copied around and which lvalueness is not required. It works, but then we're missing one big user - C++ range-based loops.

```c++
{
    for(auto& [key, obj] = wrapper.get()) {
        // ...
    }
}
```
goes into
```c++
{
    auto && __range = wrapper.get() ;
    auto __begin = std::begin(__range) ;
    auto __end = std::end(__range) ;
    for ( ; __begin != __end; ++__begin) {
        auto& [key, obj] = *__begin; // <---- oops
        // ...
    }
}
```
At "oops" label we do what's writtent above, and not something like:
```c++
    auto tmp = *__begin;
    auto key = tmp.first;
    auto& obj = tmp.second;
```
assuming that our `get()` is actually an `operator*`. 

So there several points to consider:
* store `T*` interally
* store `something` interally
* return `T&` in `get()`
* return `something` in `get()` (or `something&`) as well
* satisfy `auto& [k, v] = get()` API
* don't try to bind references to temporaries
* don't do UB
* provide user with a direct no-proxy access syntax (e.g. no `->`, etc)

Which is not achievable.

Overall picture of available options:

|User code| Internal implementation| lvalueness| # of returned objects |
|---------|------------------------|-----------|------|
|no proxy, direct access| `T&` | lvalue| 1, 2, 3, ...|
|no proxy, direct access| `T*` | lvalue| **1** |
|no proxy, direct access| `T*` | **rvalue**| 2, 3, ... |
|proxy (`->`, `.get().` ...) | `T*` | lvalue| 1, 2, 3, ...|

No-proxy and `T*` implementation is possible only when there is just one simple pointer `ptr` and `get()` doing something like `return *ptr;` thus creating lvalue ref. In case we need to return some "small" and "additional" payload, this does not work.
We don't have any kind of syntax for that:
```c++

int& Wrapper::get() {
    return *ptr; // ok, lvalue
}

char, int& Wrapper::get() {
    return ch, *ptr; // this is not C++
}

std::pair<char, int&>& Wrapper::get() {
    return {
        pair_which_stores_a_pointer_in_second.first, 
        *pair_which_stores_a_pointer_in_second.second
    }; // ref to temporary
}

std::pair<char, int&>& Wrapper::get() {
    return *reinterpret_cast<
        std::pair<char, int&>*
    >(reinterpret_cast<void*>(
        &pair_which_stores_a_pointer_in_second)); // UB
}

std::pair<char, int&> Wrapper::get() {
    return {
        pair_which_stores_a_pointer_in_second.first, 
        *pair_which_stores_a_pointer_in_second.second
    }; // a new object, no-lvalueness
       // guys who do auto& [k, v] = get(); are not happy
}
```

Thus the amount of arguments (which equals 2) in a user facing API (with no-proxy access) mandates our internal implementation to store `T&`.

Above examples describes the implementation of the view container iterator. Specifically, `Wrapper` would be an view container iterator, and `get()` is its `operator*`.

For example, in `vector_view` we store `std::vector<T*>`:
```c++
...
class vector_view ... {
    ...
    std::vector<T*> _view_container;
};
```
its `begin()` looks the following:
```c++
auto begin() {
    return view_iterator{_view_container.begin()};
}
```
which basically converts normal classical `std::vector`'s iterator into a view iterator.
What is the goal of view iterator? To do the dereference, so user doesn't need to do it for himself:
```c++
auto& sequential_iterator_view::operator*() {
    return *(Iterator::operator*());
}
```
Thus the user could do:
```c++
std::list<int> data{1, 2, 3};
vector_view view{data};
int& ref1 = *std::begin(view); // not **std::begin(view)
```

# Acknowledgements 
- Thanks for guys from Boost doing Multi-index library https://www.boost.org/doc/libs/1_62_0/libs/multi_index/doc/index.html, rviews is mostly inspired by that, but maybe tries to do only a subset of Multi-index capabilities and in a bit simpler way :)
- Thanks to Jonathan Müller and https://www.jonathanmueller.dev/talk/cppnow2018/ for structuring the pointer-like-types world in C++! Especially in the light of what we should store inside the views if we want to provide the implicit access for the user (and especially for containers which store pairs).
- Thanks to https://github.com/erikvalkering/smartref for the inspiration of the idea regarding "referenceness", and for the amazing talk in CppCon! Maybe it is possible to implement rviews on top of smartrefs?
- Thanks to https://github.com/filipdutescu/modern-cpp-template for the template, current repe structure is based on that.