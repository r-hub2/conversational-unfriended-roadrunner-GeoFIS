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
#ifndef H18D7A301_8351_456B_8DBC_EB1DFEFAE43B
#define H18D7A301_8351_456B_8DBC_EB1DFEFAE43B

#include <iterator>
#include <boost/ref.hpp>
#include <boost/range/value_type.hpp>

namespace util {

//
// /brief output iterator to back insert boost::reference_wrapper in container
//
// boost::reference_wrapper constructor is explicit (no implicit conversion) and cannot be back inserted by std::back_inserter.
//
template <class Container> class back_insert_reference_iterator {

	typedef back_insert_reference_iterator self_type;

	typedef typename boost::range_value<Container>::type container_value_type;
	typedef typename container_value_type::type type;

public:
	typedef std::output_iterator_tag 	iterator_category;
	typedef void                		value_type;
	typedef void                		difference_type;
	typedef void                		pointer;
	typedef void                		reference;

	back_insert_reference_iterator(Container &container) : container(container) {}
	back_insert_reference_iterator(const back_insert_reference_iterator &other) : container(other.container) {}

	self_type &operator=(type &value) {
		unwrap_ref(container).push_back(boost::ref(value));
		return *this;
    }

	self_type &operator*() { return *this; }
	self_type &operator++() { return *this; }
	self_type &operator++(int) { return *this; }

private:
	boost::reference_wrapper<Container> container;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Container> inline back_insert_reference_iterator<Container> back_inserter_reference(Container &container) {
	return back_insert_reference_iterator<Container>(container);
}

} // namespace util

#endif // H18D7A301_8351_456B_8DBC_EB1DFEFAE43B 
