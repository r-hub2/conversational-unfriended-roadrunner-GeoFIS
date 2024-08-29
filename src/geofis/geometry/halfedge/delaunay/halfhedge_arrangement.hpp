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
#ifndef HALFHEDGE_ARRANGEMENT_HPP_
#define HALFHEDGE_ARRANGEMENT_HPP_

#include <util/assert.hpp>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/Arrangement_2/Arrangement_on_surface_2_global.h>
#include <geofis/geometry/halfedge/delaunay/halfedge_to_ray.hpp>
#include <geofis/geometry/halfedge/delaunay/halfedge_to_line.hpp>
#include <geofis/geometry/halfedge/delaunay/halfedge_to_segment.hpp>

namespace geofis {

template <class Kernel, class Dcel, class Circulator> inline void insert(CGAL::Arrangement_2<CGAL::Arr_linear_traits_2<Kernel>, Dcel> &arrangement, Circulator circulator) {
	if(circulator->is_ray())
		CGAL::insert(arrangement, halfedge_to_ray(*circulator));
	if(circulator->is_segment())
		CGAL::insert(arrangement, halfedge_to_segment(*circulator));
	if(circulator->is_bisector())
		CGAL::insert(arrangement, halfedge_to_line(*circulator));
}

template <class Kernel, class Container, class ArrSegTraits, class Dcel, class Circulator> inline void insert(CGAL::Arrangement_2<CGAL::Gps_segment_traits_2<Kernel, Container, ArrSegTraits>, Dcel> &arrangement, Circulator circulator) {
	UTIL_REQUIRE(circulator->is_segment());
	CGAL::insert(arrangement, halfedge_to_segment(*circulator));
}

template <class Arrangement, class Circulator> inline void insert(Arrangement &arrangement, Circulator circulator, const Circulator &end) {
	CGAL_For_all(circulator, end)
		insert(arrangement, circulator);
}

} // namespace geofis

#endif /* HALFHEDGE_ARRANGEMENT_HPP_ */
