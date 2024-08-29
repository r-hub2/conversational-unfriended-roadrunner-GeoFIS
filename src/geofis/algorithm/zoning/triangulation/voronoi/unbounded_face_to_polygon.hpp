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
#ifndef UNBOUNDED_FACE_TO_POLYGON_HPP_
#define UNBOUNDED_FACE_TO_POLYGON_HPP_

#include <util/assert.hpp>
#include <CGAL/Arr_linear_traits_2.h>
#include <geofis/geometry/halfedge/delaunay/halfhedge_arrangement.hpp>
#include <geofis/geometry/halfedge/voronoi/ccb_halfedge_circulator_adaptor.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/face_to_polygon_overlay_traits.hpp>

namespace geofis {

template <class Polygon> class unbounded_face_to_polygon;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> class unbounded_face_to_polygon<CGAL::Polygon_2<Kernel> > {

	typedef CGAL::Point_2<Kernel>						point_type;
	typedef CGAL::Polygon_2<Kernel> 					polygon_type;
	typedef CGAL::Arr_linear_traits_2<Kernel>			linear_traits_type;
	typedef CGAL::Arrangement_2<linear_traits_type>		arrangement_type;

public:
	typedef CGAL::Polygon_2<Kernel> result_type;

	unbounded_face_to_polygon(const polygon_type &boundary) {
		UTIL_REQUIRE(boundary.is_simple());
		CGAL::insert(boundary_arrangement, boundary.edges_begin(), boundary.edges_end());
	}

	template <class Face> result_type operator() (Face &face) const {
		UTIL_REQUIRE(face.is_valid());
		return get_polygon(face.outer_ccb(), face.dual()->point());
	}

private:

	arrangement_type boundary_arrangement;

	template <class Circulator> result_type get_polygon(const Circulator &circulator, const point_type &face_point) const {
		arrangement_type face_arrangement;
		insert(face_arrangement, circulator, circulator);
		arrangement_type result_arrangement;
		face_to_polygon_overlay_traits<arrangement_type> overlay_traits(face_point);
		CGAL::overlay(boundary_arrangement, face_arrangement, result_arrangement, overlay_traits);
		return overlay_traits.get_polygon();
	}
};

} // namespace geofis

#endif /* UNBOUNDED_FACE_TO_POLYGON_HPP_ */
