/**
 * GeoFIS R package
 *
 * Copyright (C) 2021 INRAE
 *
 * Authors:
 * 	Jean-luc Lablée - INRAE
 * 	Serge Guillaume - INRAE
 *
 * License: CeCILL v2.1
 * 	https://cecill.info/licences/Licence_CeCILL_V2.1-en.html
 * 	https://cecill.info/licences/Licence_CeCILL_V2.1-fr.html
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "https://cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights, and the successive licensors have only limited liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef BOOST_DOUBLE_COMPARISON_H_
#define BOOST_DOUBLE_COMPARISON_H_

#include <boost/version.hpp>

#if BOOST_VERSION <= 107000
#include <boost/test/floating_point_comparison.hpp>
#else
#include <boost/test/tools/floating_point_comparison.hpp>
#endif

#if BOOST_VERSION <= 105800
namespace boost_test = boost::test_tools;
#else
namespace boost_test = boost::math::fpc;
#endif

namespace util {
namespace detail {

template <class T> T tolerance();
template <> float tolerance<float>();
template <> double tolerance<double>();
template <> long double tolerance<long double>();

template <class T> inline bool is_equal(T x, T y, T tolerance = util::detail::tolerance<T>()) { return boost_test::close_at_tolerance<T>(boost_test::percent_tolerance(tolerance))(x, y); }
template <class T> inline bool is_less(T x, T y, T tolerance = util::detail::tolerance<T>()) { return (x < y) && !is_equal(x, y, tolerance); }
template <class T> inline bool is_greater(T x, T y, T tolerance = util::detail::tolerance<T>()) { return (x > y) && !is_equal(x, y, tolerance); }
template <class T> inline bool is_less_equal(T x, T y, T tolerance = util::detail::tolerance<T>()) { return (x < y) || is_equal(x, y, tolerance); }
template <class T> inline bool is_greater_equal(T x, T y, T tolerance = util::detail::tolerance<T>()) { return (x > y) || is_equal(x, y, tolerance); }

} // namespace detail
} // namespace util

#endif /* BOOST_DOUBLE_COMPARISON_H_ */
