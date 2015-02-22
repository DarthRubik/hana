/*!
@file
Defines `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <boost/hana/fwd/set.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/insert_fwd.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // set
    //////////////////////////////////////////////////////////////////////////
    // Remove warnings generated by poor confused Doxygen
    //! @cond
    template <typename ...Xs>
    constexpr decltype(auto) _make_set::operator()(Xs&& ...xs) const {
        return detail::create<_set>{}(
                    hana::make<Tuple>(detail::std::forward<Xs>(xs)...));
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Set>
        : operators::of<Comparable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Set, Set> {
        template <typename S1, typename S2>
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            return hana::and_(
                hana::equal(hana::length(s1.storage), hana::length(s2.storage)),
                hana::subset(detail::std::forward<S1>(s1), detail::std::forward<S2>(s2))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return hana::find(detail::std::forward<Set>(set).storage,
                              detail::std::forward<Pred>(pred));
        }
    };

    template <>
    struct any_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return hana::any(detail::std::forward<Set>(set).storage,
                             detail::std::forward<Pred>(pred));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<Set, F, when<models<Foldable(F)>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::foldr(detail::std::forward<Xs>(xs),
                                set(), hana::flip(insert));
        }
    };

    template <typename L>
    struct to_impl<L, Set, when<models<Sequence(L)>{}>> {
        template <typename Set>
        static constexpr decltype(auto) apply(Set&& set)
        { return to<L>(detail::std::forward<Set>(set).storage); }
    };

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<Set> {
        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return hana::eval_if(hana::elem(set, x),
                [&set](auto) -> decltype(auto) {
                    return hana::id(detail::std::forward<S>(set));
                },
                [&set, &x](auto _) -> decltype(auto) {
                    return hana::unpack(
                        _(prepend)(
                            detail::std::forward<X>(x),
                            detail::std::forward<S>(set).storage
                        ),
                        hana::set
                    );
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
