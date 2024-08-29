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
#ifndef UNWRAP_RANGE_HPP_
#define UNWRAP_RANGE_HPP_

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/transformed.hpp>

#if BOOST_VERSION >= 108200
	#define UNWRAP_REF_BOOST_NOEXCEPT BOOST_NOEXCEPT
#else
	#define UNWRAP_REF_BOOST_NOEXCEPT
#endif

namespace util {

template <class Range> struct unwrap_range_traits {

	typedef typename boost::range_value<Range>::type range_value_type;
	typedef typename boost::range_reference<Range>::type range_reference_type;
	typedef typename boost::unwrap_reference<range_value_type>::type unwrap_reference_type;
	typedef boost::transformed_range<unwrap_reference_type &(*)(range_reference_type) UNWRAP_REF_BOOST_NOEXCEPT, Range> unwrap_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Range> inline typename unwrap_range_traits<Range>::unwrap_range_type make_unwrap_range(Range &range) {

	typedef typename boost::range_value<Range>::type range_value_type;

	return boost::adaptors::transform(range, &boost::unwrap_ref<range_value_type>);
}

template <class Range> inline typename unwrap_range_traits<const Range>::unwrap_range_type make_unwrap_range(const Range &range) {

	typedef typename boost::range_value<Range>::type range_value_type;

	return boost::adaptors::transform(range, &boost::unwrap_ref<const range_value_type>);
}

} // namespace util

#endif /* UNWRAP_RANGE_HPP_ */
