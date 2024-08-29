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
#ifndef MAKE_POLYGON_HPP_
#define MAKE_POLYGON_HPP_

#include <CGAL/Polygon_2.h>
#include <CGAL/Boolean_set_operations_2/Gps_default_traits.h>
#include <CGAL/Boolean_set_operations_2/Gps_polygon_validation.h>
#include <util/assert.hpp>
#include <boost/range.hpp>
#include <boost/bind/bind.hpp>
#include <algorithm>

namespace geofis {

template <class Kernel> inline bool is_valid_polygon(const CGAL::Polygon_2<Kernel> &polygon) {
	return CGAL::is_valid_polygon(polygon, typename CGAL::Gps_default_traits<CGAL::Polygon_2<Kernel> >::Traits());
}

template <class Kernel> inline bool is_closed_polygon(const CGAL::Polygon_2<Kernel> &polygon) {
	return CGAL::is_closed_polygon(polygon, typename CGAL::Gps_default_traits<CGAL::Polygon_2<Kernel> >::Traits());
}

template <class Kernel> inline bool is_simple_polygon(const CGAL::Polygon_2<Kernel> &polygon) {
	return CGAL::is_simple_polygon(polygon, typename CGAL::Gps_default_traits<CGAL::Polygon_2<Kernel> >::Traits());
}

template <class Kernel> inline bool has_valid_orientation_polygon(const CGAL::Polygon_2<Kernel> &polygon) {
	return CGAL::has_valid_orientation_polygon(polygon, typename CGAL::Gps_default_traits<CGAL::Polygon_2<Kernel> >::Traits());
}

template <class Kernel, class PolygonRange> inline bool is_valid_polygon_range(const PolygonRange &polygon_range) {
	return std::all_of(boost::begin(polygon_range), boost::end(polygon_range), boost::bind(is_valid_polygon<Kernel>, boost::placeholders::_1));
}

template <class Kernel, class Range> inline CGAL::Polygon_2<Kernel> make_polygon(const Range &range) {
	CGAL::Polygon_2<Kernel> polygon(boost::begin(range), boost::end(range));
	UTIL_ENSURE(is_valid_polygon(polygon));
	return polygon;
}

template <class Kernel> struct clockwise_polygon {

	typedef CGAL::Polygon_2<Kernel> result_type;

	result_type operator()(CGAL::Polygon_2<Kernel> polygon) const {
		UTIL_REQUIRE(is_valid_polygon(polygon));
		if(!polygon.is_empty() && polygon.is_counterclockwise_oriented())
			polygon.reverse_orientation();
		return polygon;
	}
};

template <class Kernel, class Iterator> inline boost::transform_iterator<clockwise_polygon<Kernel>, Iterator> make_clockwise_polygon_iterator(Iterator iterator) {
	return boost::make_transform_iterator(iterator, clockwise_polygon<Kernel>());
}

} // namespace geofis

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace CGAL {

template <class Kernel> void PrintTo(const Polygon_2<Kernel> &polygon, std::ostream *stream) {
	*stream << polygon;
}

} // namespace CGAL

#endif /* MAKE_POLYGON_HPP_ */
