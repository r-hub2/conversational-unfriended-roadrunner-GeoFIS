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
#ifndef FACE_TO_POLYGON_OVERLAY_TRAITS_HPP_
#define FACE_TO_POLYGON_OVERLAY_TRAITS_HPP_

#include <CGAL/Boolean_set_operations_2.h>
#include <geofis/geometry/circulator.hpp>
#include <geofis/geometry/polygon.hpp>
#include <geofis/geometry/halfedge/halfedge_source_point_range.hpp>

namespace geofis {

template <class Arrangement> class face_to_polygon_overlay_traits : public CGAL::Gps_base_functor<Arrangement> {

public:
	typedef typename Arrangement::Traits_2			traits_type;
	typedef typename traits_type::Kernel			kernel_type;
	typedef typename Arrangement::Point_2			point_type;
	typedef CGAL::Polygon_2<kernel_type> 			polygon_type;
	typedef typename Arrangement::Face_const_handle	face_const_handle;
	typedef typename Arrangement::Face_handle 		face_handle;

	face_to_polygon_overlay_traits(const point_type &point) : point(point) {}

	void create_face(face_const_handle, face_const_handle, face_handle face_result) {
		if(face_is_bounded(face_result) && face_has_no_hole(face_result) && face_contain_point(face_result))
			polygon = get_polygon(face_result->outer_ccb());
	}

	polygon_type get_polygon() const {
	   return polygon;
	}

private:
	point_type point;
	polygon_type polygon;

	bool face_is_bounded(const face_handle &face) {
		return !face->is_unbounded();
	}

	bool face_has_no_hole(const face_handle &face) {
		return face->number_of_holes() == 0;
	}

	bool face_contain_point(const face_handle &face) {
		return circulator_contain_point(face->outer_ccb());
	}

	template <class Circulator> bool circulator_contain_point(const Circulator &circulator) const {
		return boundary_contain_point(make_halfedge_source_point_range<kernel_type>(CGAL::Container_from_circulator<Circulator>(circulator)));
	}

	template <class Boundary> bool boundary_contain_point(const Boundary &boundary) const {
		return CGAL::bounded_side_2(boost::begin(boundary), boost::end(boundary), point) != CGAL::ON_UNBOUNDED_SIDE;
	}

	template <class Circulator> polygon_type get_polygon(const Circulator &circulator) const {
		return make_polygon<kernel_type>(make_halfedge_source_point_range<kernel_type>(CGAL::Container_from_circulator<Circulator>(circulator)));
	}
};

} // namespace geofis

#endif /* FACE_TO_POLYGON_OVERLAY_TRAITS_HPP_ */
