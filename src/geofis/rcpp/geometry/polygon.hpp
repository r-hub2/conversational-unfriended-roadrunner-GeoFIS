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
#ifndef HB202CFF6_1D06_4516_8C79_56960624A42C
#define HB202CFF6_1D06_4516_8C79_56960624A42C

#include <Rcpp.h>
#include <CGAL/Polygon_2.h>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/join.hpp>
#include <geofis/geometry/point.hpp>
#include <vector>

namespace geofis {

template <class Kernel> Rcpp::S4 make_rcpp_polygon(const CGAL::Polygon_2<Kernel> &polygon, bool is_hole) {

	typedef typename CGAL::Polygon_2<Kernel>::Point_2 point_type;

	std::vector<point_type> points(polygon.vertices_begin(), polygon.vertices_end());
	points.push_back(polygon[0]);
	auto x_range = points | boost::adaptors::transformed(point_to_double_x());
	auto y_range = points | boost::adaptors::transformed(point_to_double_y());
	auto coord_range = boost::join(x_range, y_range);
	Rcpp::NumericMatrix coords(polygon.size() + 1, 2, boost::begin(coord_range));
	colnames(coords) = Rcpp::CharacterVector::create("x", "y");
	Rcpp::Function polygon_function("Polygon");
	return polygon_function(Rcpp::Named("coords") = coords, Rcpp::Named("hole") = is_hole);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct rcpp_polygon_maker {

	typedef Rcpp::S4 result_type;

	bool is_hole;

	rcpp_polygon_maker() : rcpp_polygon_maker(false) {}
	rcpp_polygon_maker(bool is_hole) : is_hole(is_hole) {}

	template <class Kernel> result_type operator() (const CGAL::Polygon_2<Kernel> &polygon) const {
		return make_rcpp_polygon(polygon, is_hole);
	}
};

} // namespace geofis

#endif // HB202CFF6_1D06_4516_8C79_56960624A42C 
