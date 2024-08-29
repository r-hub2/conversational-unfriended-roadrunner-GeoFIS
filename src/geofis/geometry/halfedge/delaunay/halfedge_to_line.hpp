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
#ifndef HALFEDGE_TO_LINE_HPP_
#define HALFEDGE_TO_LINE_HPP_

#include <util/assert.hpp>
#include <CGAL/Triangulation_utils_2.h>
#include <CGAL/Kernel/global_functions_2.h>

namespace geofis {

template <class Kernel, class Vertex> inline CGAL::Line_2<Kernel> delaunay_vertices_to_line(const Vertex &vertex1, const Vertex &vertex2) {
	return CGAL::bisector(vertex1->point(), vertex2->point());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel, class DelaunayEdge> inline CGAL::Line_2<Kernel> delaunay_edge_to_line(const DelaunayEdge &delaunay_edge) {
	return delaunay_vertices_to_line<Kernel>(delaunay_edge.first->vertex(CGAL::Triangulation_cw_ccw_2::cw(delaunay_edge.second)), delaunay_edge.first->vertex(CGAL::Triangulation_cw_ccw_2::ccw(delaunay_edge.second)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Halfedge> inline CGAL::Line_2<typename Halfedge::Delaunay_graph::Geom_traits> halfedge_to_line(const Halfedge &halfedge) {
	UTIL_REQUIRE(halfedge.is_bisector());
	return delaunay_edge_to_line<typename Halfedge::Delaunay_graph::Geom_traits>(halfedge.dual());
}

} // namespace geofis

#endif /* HALFEDGE_TO_LINE_HPP_ */
