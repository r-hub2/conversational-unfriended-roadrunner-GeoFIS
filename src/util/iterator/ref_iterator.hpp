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
#ifndef REF_ITERATOR_HPP_
#define REF_ITERATOR_HPP_

#include <boost/ref.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace util {

template <class T> struct referencer {

	typedef boost::reference_wrapper<T> result_type;

	result_type operator() (T &t) const {
		return boost::reference_wrapper<T>(t);
	}
};

template <class Iterator> struct ref_iterator_traits {

	typedef typename std::iterator_traits<Iterator>::reference reference_type;
	typedef typename boost::remove_reference<reference_type>::type value_type;
	typedef boost::transform_iterator<referencer<value_type>, Iterator> ref_iterator_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Iterator> inline typename ref_iterator_traits<Iterator>::ref_iterator_type make_ref_iterator(const Iterator &iterator) {

	typedef typename ref_iterator_traits<Iterator>::value_type value_type;

	return boost::make_transform_iterator(iterator, referencer<value_type>());
}

} // namespace util

#endif /* REF_ITERATOR_HPP_ */
