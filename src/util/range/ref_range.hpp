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
#ifndef REF_RANGE_HPP_
#define REF_RANGE_HPP_

#include <boost/range.hpp>
#include <util/iterator/ref_iterator.hpp>

namespace util {

template <class Range> struct ref_range_traits {

	typedef typename boost::range_iterator<Range>::type iterator_type;
	typedef typename ref_iterator_traits<iterator_type>::ref_iterator_type ref_iterator_type;
	typedef boost::iterator_range<ref_iterator_type> iterator_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Range> class ref_range : public ref_range_traits<Range>::iterator_range_type {

	typedef typename ref_range_traits<Range>::iterator_range_type iterator_range_type;

public:
	ref_range(Range &range) : iterator_range_type(make_ref_iterator(boost::begin(range)), make_ref_iterator(boost::end(range))) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Range> inline ref_range<Range> make_ref_range(Range &range) {
	return ref_range<Range>(range);
}

template <class Range> inline ref_range<const Range> make_ref_range(const Range &range) {
	return ref_range<const Range>(range);
}

} // namespace util

#endif /* REF_RANGE_HPP_ */
