#pragma once

/* Explicitly create all functions during template instansiation
 * in order to obtain the full code coverage
 */
#ifdef VIEW_DEBUG
#define VIEW_API __attribute__((__used__))
#else
#define VIEW_API
#endif

namespace rviews {

template <typename C>
struct deinstantiate;

template <template <class...> class C, typename... T1s>
struct deinstantiate<C<T1s...>> {
    template <typename... T2s>
    using type = C<T2s...>;
};

template <typename... T>  // declaration only for TD;
class TD;

/*
    In debug:
        ::rviews::cout == ::std::cout
        ::rviews::printf == ::std::printf
    In production:
        ::rviews::cout == nothing
        ::rviews::printf == nothing
    Please don't use std::endl (instead use "\n"), since we don't
    want here to inject cout_type into C++ streams system with
    inheritance.
*/
#ifdef VIEW_DEBUG
static auto& cout = std::cout;
#else
struct cout_type {};  // dummy type

static cout_type cout;  // dummy object

template <typename T>
static cout_type& operator<<(cout_type& stream, [[maybe_unused]] T&& whatever) {
    return stream;  // do nothing, to be optimized out
}
#endif

template <typename... Types>
void printf([[maybe_unused]] Types... types) {
#ifdef VIEWS_DEBUG
    ::printf(types...);
#else
    ;
#endif
}

}  // namespace rviews
