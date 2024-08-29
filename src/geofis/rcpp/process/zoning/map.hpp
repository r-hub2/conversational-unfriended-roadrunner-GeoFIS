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
#ifndef H39514393_1809_4FB0_937C_EAEF7B6602E5
#define H39514393_1809_4FB0_937C_EAEF7B6602E5

#include <Rcpp.h>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/rcpp/geometry/polygons.hpp>

namespace geofis {

namespace detail {

template <class Map> Rcpp::S4 make_rcpp_map_spatial_polygons(const Map &map, const Rcpp::S4 &crs) {
	auto geometry_range = make_geometry_range(map.get_zones());
	Rcpp::List Srl = make_rcpp_polygons_list(geometry_range);
	Rcpp::Function rebuild_crs("rebuild_CRS");
	Rcpp::S4 proj4string = rebuild_crs(crs);
	Rcpp::Function spatial_polygons("SpatialPolygons");
	return spatial_polygons(Rcpp::Named("Srl") = Srl, Rcpp::Named("proj4string") = proj4string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Map> Rcpp::DataFrame make_rcpp_map_data_frame(const Map &map, const Rcpp::CharacterVector &col_names) {
	auto zone_range = map.get_zones();
	auto id_range = zone_range | boost::adaptors::transformed([](const auto &zone) { return zone.get_id(); });
	Rcpp::CharacterVector ids(boost::begin(id_range), boost::end(id_range));
	auto size_range = zone_range | boost::adaptors::transformed([](const auto &zone) { return zone.size(); });
	Rcpp::IntegerVector sizes(boost::begin(size_range), boost::end(size_range));
	auto area_range = zone_range | boost::adaptors::transformed([](const auto &zone) { return zone.get_area(); });
	Rcpp::NumericVector areas(boost::begin(area_range), boost::end(area_range));
	std::vector<SEXP> column_range = { ids, sizes, areas };
	for(R_xlen_t column = 0; column < col_names.size(); column++) {
		auto mean_range = zone_range | boost::adaptors::transformed([column](const auto &zone) { return zone.get_mean(column); });
		Rcpp::NumericVector means(boost::begin(mean_range), boost::end(mean_range));
		column_range.push_back(means);
		auto std_range = zone_range | boost::adaptors::transformed([column](const auto &zone) { return zone.get_standard_deviation(column); });
		Rcpp::NumericVector stds(boost::begin(std_range), boost::end(std_range));
		column_range.push_back(stds);
	}
	Rcpp::List columns(boost::begin(column_range), boost::end(column_range));
	Rcpp::DataFrame data_frame(columns);

	// column names of the data frame
	std::vector<std::string> name_range = { "id", "size", "area" };
	BOOST_FOREACH(auto col_name, col_names) {
		name_range.push_back(std::string(col_name) + "_mean");
		name_range.push_back(std::string(col_name) + "_std");
	}
	Rcpp::CharacterVector names(boost::begin(name_range), boost::end(name_range));
	data_frame.attr("names") = names;
	return data_frame;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct rcpp_map_maker {

	typedef Rcpp::S4 result_type;

	Rcpp::S4 crs;
	Rcpp::CharacterVector col_names;

	rcpp_map_maker(const Rcpp::S4 &crs, const Rcpp::CharacterVector &col_names) : crs(crs), col_names(col_names) {}

	template <class Map> result_type operator() (const Map &map) const {
		return make_rcpp_map(map, crs, col_names);
	}
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Map> Rcpp::S4 make_rcpp_map(const Map &map, const Rcpp::S4 &crs, const Rcpp::CharacterVector &col_names) {
	Rcpp::S4 Sr = detail::make_rcpp_map_spatial_polygons(map, crs);
	Rcpp::DataFrame data = detail::make_rcpp_map_data_frame(map, col_names);
	Rcpp::Function spatial_polygons_data_frame("SpatialPolygonsDataFrame");
	return spatial_polygons_data_frame(Rcpp::Named("Sr") = Sr, Rcpp::Named("data") = data, Rcpp::Named("match.ID") = false);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class MapRange> Rcpp::List make_rcpp_map_list(const MapRange &map_range, const Rcpp::S4 &crs, const Rcpp::CharacterVector &col_names) {
	auto rcpp_map_range = map_range | boost::adaptors::transformed(detail::rcpp_map_maker(crs, col_names));
	return Rcpp::List(boost::begin(rcpp_map_range), boost::end(rcpp_map_range));
}

} // namespace geofis

#endif // H39514393_1809_4FB0_937C_EAEF7B6602E5 
