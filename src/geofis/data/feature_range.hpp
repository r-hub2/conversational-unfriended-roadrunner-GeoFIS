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
#ifndef FEATURE_RANGE_HPP_
#define FEATURE_RANGE_HPP_

#include <geofis/data/feature_iterator.hpp>

namespace geofis {

template <class GeometryRange, class AttributeRange1> struct feature_range_traits {

	typedef typename boost::range_iterator<GeometryRange>::type geometry_iterator;
	typedef typename boost::range_iterator<AttributeRange1>::type attribute_iterator;
	typedef feature_iterator<geometry_iterator, attribute_iterator> feature_iterator_type;
	typedef boost::iterator_range<feature_iterator_type> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryRange, class AttributeRange1> class feature_range : public feature_range_traits<GeometryRange, AttributeRange1>::base_type {

	typedef typename feature_range_traits<GeometryRange, AttributeRange1>::base_type base_type;

public:
	feature_range(const GeometryRange &geometry_range, const AttributeRange1 &attribute_range1) : base_type(make_feature_iterator(boost::begin(geometry_range), boost::begin(attribute_range1)), make_feature_iterator(boost::end(geometry_range), boost::end(attribute_range1))) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryRange, class AttributeRange1> inline feature_range<GeometryRange, AttributeRange1> make_feature_range(const GeometryRange &geometry_range, const AttributeRange1 &attribute_range1) {
	return feature_range<GeometryRange, AttributeRange1>(geometry_range, attribute_range1);
}

} // namespace geofis

#endif /* FEATURE_RANGE_HPP_ */
