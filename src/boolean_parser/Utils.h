#ifndef LOCK_FREE_LIST_UTILS_H_
#define LOCK_FREE_LIST_UTILS_H_

#include <boost/variant.hpp>
#include <boost/spirit/home/x3.hpp>
#include <type_traits>


namespace parser::utils {
template <typename T>
struct HoldsAlternativeImpl {
    using result_type = bool;

    template <typename U>
    bool operator()(U const &) const {
        return std::is_same_v<U, T>;
    }
};

template <typename T, typename... Ts>
inline bool holdsAlternative(boost::spirit::x3::variant<Ts...> const &v) {
    return boost::apply_visitor(HoldsAlternativeImpl<T>(), v);
}

// Boolean basis

template <typename T>
concept BooleanType = requires(T a, T b) {
    { a || b } -> std::convertible_to<T>;
    { a && b } -> std::convertible_to<T>;
    { !a } -> std::convertible_to<T>;
};

template <BooleanType T>
inline T unaryNot(T x) {
    return !x;
}

template <BooleanType T>
inline T logicalAnd(T x, T y) {
    return x && y;
}

template <BooleanType T>
inline T logicalOr(T x, T y) {
    return x || y;
}
}   // namespace parser::utils

#endif  // LOCK_FREE_LIST_UTILS_H_
