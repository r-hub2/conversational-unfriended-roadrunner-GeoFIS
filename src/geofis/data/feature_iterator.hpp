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
#ifndef FEATURE_ITERATOR_HPP_
#define FEATURE_ITERATOR_HPP_

#include <geofis/data/feature.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace geofis {

template <class GeometryIterator, class AttributeIterator1> class feature_iterator;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryIterator, class AttributeIterator1> struct feature_iterator_traits {

	typedef typename std::iterator_traits<GeometryIterator>::value_type geometry_type;

	typedef typename AttributeIterator1::dual_iterator_type attribute_iterator2;
	typedef boost::iterator_range<attribute_iterator2> attribute_range2;

	typedef feature<size_t, geometry_type, attribute_range2> feature_type;
	typedef boost::iterator_facade<feature_iterator<GeometryIterator, AttributeIterator1>, feature_type, std::input_iterator_tag, feature_type> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryIterator, class AttributeIterator1> class feature_iterator : public feature_iterator_traits<GeometryIterator, AttributeIterator1>::base_type {

	friend class boost::iterator_core_access;

	typedef typename feature_iterator_traits<GeometryIterator, AttributeIterator1>::feature_type feature_type;

public:
	feature_iterator(const GeometryIterator &geometry_iterator, const AttributeIterator1 &attribute_iterator1) : geometry_iterator(geometry_iterator), attribute_iterator1(attribute_iterator1) {}

private:
	GeometryIterator geometry_iterator;
	AttributeIterator1 attribute_iterator1;

    void increment() {
    	++geometry_iterator;
    	++attribute_iterator1;
    }

    bool equal(const feature_iterator &other) const {
    	return (geometry_iterator == other.geometry_iterator) && (attribute_iterator1 == other.attribute_iterator1);
    }

    feature_type dereference() const { return feature_type(attribute_iterator1.index1(), *geometry_iterator, boost::make_iterator_range(attribute_iterator1.begin(), attribute_iterator1.end())); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryIterator, class AttributeIterator1> inline feature_iterator<GeometryIterator, AttributeIterator1> make_feature_iterator(const GeometryIterator &geometry_iterator, const AttributeIterator1 &attribute_iterator1) {
	return feature_iterator<GeometryIterator, AttributeIterator1>(geometry_iterator, attribute_iterator1);
}

} // namespace geofis

#endif /* FEATURE_ITERATOR_HPP_ */
