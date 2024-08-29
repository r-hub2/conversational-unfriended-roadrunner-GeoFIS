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
#ifndef ZONE_JOIN_HPP_
#define ZONE_JOIN_HPP_

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Lazy_exact_nt.h>

namespace geofis {

template <class Zone> struct zone_join_traits {

	typedef typename Zone::geometry_type geometry_type;
	typedef typename CGAL::Gps_default_traits<geometry_type>::Traits traits_type;
	typedef CGAL::General_polygon_set_2<traits_type> general_polygon_set;
	typedef typename general_polygon_set::Arrangement_2 arrangement_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone> bool zone_joinable(const Zone &zone1, const Zone &zone2) {

	typedef typename zone_join_traits<Zone>::general_polygon_set general_polygon_set;
	typedef typename zone_join_traits<Zone>::arrangement_type arrangement_type;

	general_polygon_set gps1(zone1.get_geometry());
	general_polygon_set gps2(zone2.get_geometry());
	arrangement_type arrangement_result;
	CGAL::Gps_do_intersect_functor<arrangement_type> overlay_traits;
	CGAL::overlay(gps1.arrangement(), gps2.arrangement(), arrangement_result, overlay_traits);
	return overlay_traits.found_boundary_intersection() && !overlay_traits.found_reg_intersection();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Arrangement> class join_length_overlay_traits : public CGAL::Gps_do_intersect_functor<Arrangement> {

public:
	typedef CGAL::Gps_do_intersect_functor<Arrangement> 		base_type;
	typedef typename Arrangement::Face_const_handle       		face_const_handel;
	typedef typename Arrangement::Halfedge_const_handle   		halfedge_const_handle;
	typedef typename Arrangement::Halfedge_handle    			halfedge_handle;
	typedef typename Arrangement::Geometry_traits_2::Kernel::RT	RT;

	join_length_overlay_traits() : base_type(), join_length(0) {}

	void create_edge(halfedge_const_handle edge1, halfedge_const_handle edge2, halfedge_handle result) {
		base_type::create_edge(edge1, edge2, result);
		join_length += halfedge_length(result);
	}

	void create_edge(halfedge_const_handle edge, face_const_handel face, halfedge_handle result) { base_type::create_edge(edge, face, result); }
	void create_edge(face_const_handel face , halfedge_const_handle edge, halfedge_handle result) { base_type::create_edge(face, edge, result); }

	double get_join_length() const {
		return base_type::found_boundary_intersection() && !base_type::found_reg_intersection() ? join_length : -1;
	}

private:
	double join_length;

	double distance(double squared_distance) { return sqrt(squared_distance); }

	template <class ET> double distance(const CGAL::Lazy_exact_nt<ET> &squared_distance) {
		return distance(CGAL::to_double(squared_distance));
	}

	template <class Point> double distance(const Point &source, const Point &target) {
		return distance(CGAL::squared_distance(source, target));
	}

	double halfedge_length(halfedge_handle halfedge) {
		return distance(halfedge->source()->point(), halfedge->target()->point());
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone> double zone_join_length(const Zone &zone1, const Zone &zone2) {

	typedef typename zone_join_traits<Zone>::general_polygon_set general_polygon_set;
	typedef typename zone_join_traits<Zone>::arrangement_type arrangement_type;

	general_polygon_set gps1(zone1.get_geometry());
	general_polygon_set gps2(zone2.get_geometry());
	arrangement_type arrangement_result;
	join_length_overlay_traits<arrangement_type> overlay_traits;
	CGAL::overlay(gps1.arrangement(), gps2.arrangement(), arrangement_result, overlay_traits);
	return overlay_traits.get_join_length();
}

} // namespace geofis

#endif /* ZONE_JOIN_HPP_ */
