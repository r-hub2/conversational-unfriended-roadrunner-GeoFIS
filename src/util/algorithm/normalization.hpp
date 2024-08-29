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
#ifndef NORMALIZATION_HPP_
#define NORMALIZATION_HPP_

#include <util/assert.hpp>
#include <boost/range/size.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/algorithm/minmax_element.hpp>

namespace util {

template <class T> class normalize_function {

public:
	normalize_function(const T &min, const T &max) : min(min), delta(max - min) {
		UTIL_REQUIRE(delta > 0);
	}

	T operator()(const T &value) const {
		return (value - min) / delta;
	}

private:
	T min;
	T delta;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> struct normalize_function_maker {

	typedef normalize_function<T> result_type;

	result_type operator()(const T &min, const T &max) const {
		return result_type(min, max);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> class unnormalize_function {

public:
	unnormalize_function(const T &min, const T &max) : min(min), delta(max - min) {
		UTIL_REQUIRE(delta > 0);
	}

	T operator()(const T &value) const {
		return min + (value * delta);
	}

private:
	T min;
	T delta;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> struct unnormalize_function_maker {

	typedef unnormalize_function<T> result_type;

	result_type operator()(const T &min, const T &max) const {
		return result_type(min, max);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> class normalization {

public:
	template <class Range> static normalization initialize(const Range &range) {
		UTIL_REQUIRE(boost::size(range) > 1);
		return initialize(boost::minmax_element(boost::begin(range), boost::end(range)));
	}

	template <class Range> void normalize(Range &range) const {
		normalize(range, boost::begin(range));
	}

	template <class Range, class OutputIterator> void normalize(const Range &range, OutputIterator output) const {
		boost::transform(range, output, normalizer);
	}

	template <class Range> void unnormalize(Range &range) const {
		unnormalize(range, boost::begin(range));
	}

	template <class Range, class OutputIterator> void unnormalize(const Range &range, OutputIterator output) const {
		boost::transform(range, output, unnormalizer);
	}

private:
	normalize_function<T> normalizer;
	unnormalize_function<T> unnormalizer;

	normalization(normalize_function<T> normalizer,	unnormalize_function<T> unnormalizer) : normalizer(normalizer), unnormalizer(unnormalizer) {}

	template <class Iterator> static normalization initialize(const std::pair<Iterator, Iterator> &min_max) {
		return initialize(*min_max.first, *min_max.second);
	}

	static normalization initialize(const T &min, const T &max) {
		return normalization(normalize_function<T>(min, max), unnormalize_function<T>(min, max));
	}
};

} // namespace util

#endif /* NORMALIZATION_HPP_ */
