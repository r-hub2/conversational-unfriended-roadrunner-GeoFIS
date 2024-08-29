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
#ifndef MINKOWSKI_DISTANCE_HPP_
#define MINKOWSKI_DISTANCE_HPP_

#include <cmath>
#include <algorithm>
#include <util/assert.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace util {

template <class T> struct minkowski_distance {

	typedef T result_type;

	minkowski_distance() : power(2) {}
	minkowski_distance(T power) : power(power) {}

	struct element_distance {

		typedef T result_type;

		element_distance(T power) : power(power) {}

		result_type operator()(T lhs, T rhs) const {
			return std::pow(std::abs(lhs - rhs), power);
		}

		result_type operator()(T x) const {
			return std::pow(std::abs(x), power);
		}

		T power;
	};

	template <class Range1, class Range2> result_type operator()(const Range1 &range1, const Range2 &range2) const {
		BOOST_MPL_ASSERT((boost::is_convertible<typename boost::range_value<Range1>::type, T>));
		BOOST_MPL_ASSERT((boost::is_convertible<typename boost::range_value<Range2>::type, T>));
		UTIL_REQUIRE(boost::size(range1) == boost::size(range2));
		return std::pow(boost::inner_product(range1, range2, (T)0, std::plus<T>(), element_distance(power)), (T)1 / power);
	}

	template <class Range> result_type operator()(const Range &range) const {
		BOOST_MPL_ASSERT((boost::is_convertible<typename boost::range_value<Range>::type, T>));
		return std::pow(boost::accumulate(boost::adaptors::transform(range, element_distance(power)), (T)0), (T)1 / power);
	}

	bool operator==(const minkowski_distance &other) {
		return power == other.power;
	}

	T power;
};

} // namespace util

#endif /* MINKOWSKI_DISTANCE_HPP_ */
