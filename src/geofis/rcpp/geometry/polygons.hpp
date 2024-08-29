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
#ifndef H80DC862E_9E3E_405C_AB2E_45FC39A0C9C4
#define H80DC862E_9E3E_405C_AB2E_45FC39A0C9C4

#include <Rcpp.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign/list_of.hpp>
#include <geofis/rcpp/geometry/polygon.hpp>

namespace geofis {

template <class Kernel> Rcpp::S4 make_rcpp_polygons(const CGAL::Polygon_2<Kernel> &polygon, std::string id, bool is_hole = false) {
	Rcpp::S4 rcpp_polygon = make_rcpp_polygon(polygon, is_hole);
	Rcpp::List srl = { rcpp_polygon };
	Rcpp::Function polygons_function("Polygons");
	Rcpp::S4 polygons = polygons_function(Rcpp::Named("srl") = srl, Rcpp::Named("ID") = id);
	polygons.attr("comment") = "0";
	return polygons;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> Rcpp::S4 make_rcpp_polygons(const CGAL::Polygon_with_holes_2<Kernel> &polygon_with_holes, std::string id) {
	Rcpp::S4 rcpp_outer_polygon = make_rcpp_polygon(polygon_with_holes.outer_boundary(), false);
	auto hole_range = boost::make_iterator_range(polygon_with_holes.holes_begin(), polygon_with_holes.holes_end());
	auto polygon_hole_range = hole_range | boost::adaptors::transformed(rcpp_polygon_maker(true));
	auto polygon_range = boost::assign::list_of(rcpp_outer_polygon).range(polygon_hole_range);
	Rcpp::List srl(boost::begin(polygon_range), boost::end(polygon_range));
	Rcpp::Function polygons_function("Polygons");
	Rcpp::S4 polygons = polygons_function(Rcpp::Named("srl") = srl, Rcpp::Named("ID") = id);
	std::vector<std::string> ownerships = boost::assign::list_of("0").repeat(polygon_with_holes.number_of_holes(), "1");
	polygons.attr("comment") = boost::algorithm::join(ownerships, " ");
	return polygons;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct rcpp_polygons_maker {

	typedef Rcpp::S4 result_type;

	template <class Indexed> result_type operator() (const Indexed &indexed) const {
		return make_rcpp_polygons(indexed.value(), std::to_string(indexed.index()));
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class PolygonRange> Rcpp::List make_rcpp_polygons_list(const PolygonRange &polygon_range) {
	auto rcpp_polygon_range = polygon_range | boost::adaptors::indexed(1) | boost::adaptors::transformed(rcpp_polygons_maker());
	return Rcpp::List(boost::begin(rcpp_polygon_range), boost::end(rcpp_polygon_range));
}

} // namespace geofis

#endif // H80DC862E_9E3E_405C_AB2E_45FC39A0C9C4 
