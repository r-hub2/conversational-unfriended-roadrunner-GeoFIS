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
#ifndef H3C086B96_2DC8_4A64_A1ED_189CE8D4F35D
#define H3C086B96_2DC8_4A64_A1ED_189CE8D4F35D

#include <geofis/geometry/area/geometry_area_traits.hpp>
#include <CGAL/number_utils.h>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace geofis {

template <class Geometry> inline typename geometry_area_traits<Geometry>::area_type get_geometry_area(const Geometry &geometry) {
	return geometry.area();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> inline typename geometry_area_traits<CGAL::Polygon_2<Kernel> >::area_type get_geometry_area(const CGAL::Polygon_2<Kernel> &polygon) {
	return polygon.area();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryRange> typename geometry_area_range_traits<GeometryRange>::area_type get_geometries_area(const GeometryRange &geometries);

template <class Kernel> inline typename geometry_area_traits<CGAL::Polygon_with_holes_2<Kernel> >::area_type get_geometry_area(const CGAL::Polygon_with_holes_2<Kernel> &polygon_with_holes) {
	// area is negative for clockwise polygon (see documentation of Polygon_2 area).
	// so the sum of holes is negative and added to outer_boundary area.
	return get_geometry_area(polygon_with_holes.outer_boundary()) + get_geometries_area(boost::make_iterator_range(polygon_with_holes.holes_begin(), polygon_with_holes.holes_end()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <class Geometry> struct geometry_area_getter {

	typedef typename geometry_area_traits<Geometry>::area_type result_type;

	result_type operator()(const Geometry &geometry) const {
		return get_geometry_area(geometry);
	}
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryRange> inline typename geometry_area_range_traits<GeometryRange>::area_type get_geometries_area(const GeometryRange &geometries) {

	typedef typename geometry_area_range_traits<GeometryRange>::area_type area_type;
	typedef typename geometry_area_range_traits<GeometryRange>::geometry_type geometry_type;

	return boost::accumulate(boost::adaptors::transform(geometries, detail::geometry_area_getter<geometry_type>()), area_type(0.0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry> inline double get_double_geometry_area(const Geometry &geometry) {
	return get_geometry_area(geometry);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> inline double get_double_geometry_area(const CGAL::Polygon_2<Kernel> &polygon) {
	return CGAL::to_double(get_geometry_area(polygon));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> inline double get_double_geometry_area(const CGAL::Polygon_with_holes_2<Kernel> &polygon_with_holes) {
	return CGAL::to_double(get_geometry_area(polygon_with_holes));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometryRange> inline double get_double_geometries_area(const GeometryRange &geometries) {
	return CGAL::to_double(get_geometries_area(geometries));
}

} // namespace geofis

#endif // H3C086B96_2DC8_4A64_A1ED_189CE8D4F35D 
