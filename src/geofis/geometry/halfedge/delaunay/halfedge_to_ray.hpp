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
#ifndef HALFEDGE_TO_RAY_HPP_
#define HALFEDGE_TO_RAY_HPP_

#include <util/assert.hpp>
#include <CGAL/Ray_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Direction_2.h>

namespace geofis {

template <class Halfedge> inline CGAL::Point_2<typename Halfedge::Delaunay_graph::Geom_traits> get_ray_base(const Halfedge &halfedge) {
	UTIL_REQUIRE(halfedge.is_ray());
	return halfedge.has_source() ? halfedge.source()->point() : halfedge.target()->point();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> inline CGAL::Direction_2<Kernel> get_ray_direction(const CGAL::Point_2<Kernel> &point1, const CGAL::Point_2<Kernel> &point2) {
	return CGAL::Segment_2<Kernel>(point1, point2).to_vector().perpendicular(CGAL::CLOCKWISE).direction();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Halfedge> inline CGAL::Direction_2<typename Halfedge::Delaunay_graph::Geom_traits> get_ray_direction(const Halfedge &halfedge) {
	UTIL_REQUIRE(halfedge.is_ray());
	return halfedge.has_source() ? get_ray_direction(halfedge.down()->point(), halfedge.up()->point()) : get_ray_direction(halfedge.up()->point(), halfedge.down()->point());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Halfedge> inline CGAL::Ray_2<typename Halfedge::Delaunay_graph::Geom_traits> halfedge_to_ray(const Halfedge &halfedge) {
	UTIL_REQUIRE(halfedge.is_ray());
	return CGAL::Ray_2<typename Halfedge::Delaunay_graph::Geom_traits>(get_ray_base(halfedge), get_ray_direction(halfedge));
}

} // namespace geofis

#endif /* HALFEDGE_TO_RAY_HPP_ */
