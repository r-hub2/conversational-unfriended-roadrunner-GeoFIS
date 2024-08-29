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
#ifndef HD36A2CF8_32B9_40A4_87A2_8A0B4C619EC3
#define HD36A2CF8_32B9_40A4_87A2_8A0B4C619EC3

#include <Rcpp.h>
#include <boost/assign/std/vector.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/join.hpp>
#include <geofis/rcpp/geometry/lines.hpp>

namespace geofis {

namespace detail {

struct rcpp_lines_maker {

	typedef Rcpp::S4 result_type;

	template <class Indexed> result_type operator() (const Indexed &indexed) const {
		auto &neighbor = indexed.value();
		auto point1 = neighbor.get_zone1().get_feature(0).get_geometry();
		auto point2 = neighbor.get_zone2().get_feature(0).get_geometry();
		return make_rcpp_lines(point1, point2, std::to_string(indexed.index()));
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class NeighborRange> Rcpp::S4 make_rcpp_map_spatial_lines(const NeighborRange &neighbors, const NeighborRange &filtered_neighbors, const Rcpp::S4 &crs) {
	auto lines_range = neighbors | boost::adaptors::indexed(1) | boost::adaptors::transformed(rcpp_lines_maker());
	auto filtered_lines_range = filtered_neighbors | boost::adaptors::indexed(boost::distance(neighbors) + 1) | boost::adaptors::transformed(rcpp_lines_maker());
	auto joined_lines = boost::join(lines_range, filtered_lines_range);
	Rcpp::List LinesList(boost::begin(joined_lines), boost::end(joined_lines));
	Rcpp::Function rebuild_crs("rebuild_CRS");
	Rcpp::S4 proj4string = rebuild_crs(crs);
	Rcpp::Function spatial_lines("SpatialLines");
	return spatial_lines(Rcpp::Named("LinesList") = LinesList, Rcpp::Named("proj4string") = proj4string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class NeighborRange> Rcpp::DataFrame make_rcpp_neighborhood_data_frame(const NeighborRange &neighbors, const NeighborRange &filtered_neighbors) {
	std::vector<bool> filtered_values;
	boost::assign::push_back(filtered_values).repeat(boost::distance(neighbors), false).repeat(boost::distance(filtered_neighbors), true);
	Rcpp::LogicalVector filtered(boost::begin(filtered_values), boost::end(filtered_values));
	return Rcpp::DataFrame::create(Rcpp::Named("filtered") = filtered);
}

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class NeighborRange> Rcpp::S4 make_rcpp_neighborhood_map(const NeighborRange &neighbors, const NeighborRange &filtered_neighbors, const Rcpp::S4 &crs) {
	Rcpp::S4 sl =  detail::make_rcpp_map_spatial_lines(neighbors, filtered_neighbors, crs);
	Rcpp::DataFrame data = detail::make_rcpp_neighborhood_data_frame(neighbors, filtered_neighbors);
	Rcpp::Function spatial_lines_data_frame("SpatialLinesDataFrame");
	return spatial_lines_data_frame(Rcpp::Named("sl") = sl, Rcpp::Named("data") = data, Rcpp::Named("match.ID") = false);
}

} // namespace geofis

#endif // HD36A2CF8_32B9_40A4_87A2_8A0B4C619EC3
