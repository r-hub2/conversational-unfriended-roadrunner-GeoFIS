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
#include <Rcpp.h>
#include <util/functional/maximum.hpp>
#include <util/functional/minimum.hpp>
#include <util/functional/mean.hpp>
#include <util/functional/distance/euclidean_distance.hpp>
#include <util/functional/distance/minkowski_distance.hpp>
#include <base/fuzzy_distance.hpp>
#include <geofis/algorithm/zoning/merge/size/size_merge.hpp>
#include <geofis/algorithm/zoning/merge/area/area_merge.hpp>
#include <zoning_wrapper.h>
#include <fisin_wrapper.h>

using namespace Rcpp;
using namespace std;
using namespace util;
using namespace fispro;
using namespace geofis;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_s4_class(SEXP arg, const string &clazz) {
	if(Rf_isS4(arg)) {
		S4 s4 = arg;
		return s4.is(clazz);
	} else
		return false;
}

bool is_minimum_zone_distance(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_minimum_wrapper");
}

bool is_maximum_zone_distance(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_maximum_wrapper");
}

bool is_mean_zone_distance(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_mean_wrapper");
}

bool is_euclidean_combine_distance(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_euclidean_wrapper");
}

bool is_minkowski_combine_distance(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_minkowski_wrapper");
}

bool is_size_merge(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_size_wrapper");
}

bool is_area_merge(SEXP* args, int nargs) {
	return is_s4_class(args[0], "Rcpp_area_wrapper");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RCPP_EXPOSED_AS(minimum<double>)

RCPP_EXPOSED_AS(maximum<double>)

RCPP_EXPOSED_AS(util::mean<double>)

RCPP_EXPOSED_AS(euclidean_distance<double>)

RCPP_EXPOSED_AS(minkowski_distance<double>)

RCPP_EXPOSED_AS(fuzzy_distance)

RCPP_EXPOSED_AS(size_merge)

RCPP_EXPOSED_AS(area_merge)

RCPP_EXPOSED_AS(fisin_wrapper)

RCPP_MODULE(zoning_module) {

	class_<zoning_wrapper>("zoning_wrapper")
	.constructor<S4>()
	//.constructor<StringVector, NumericMatrix, NumericMatrix>()
	.method("set_border", &zoning_wrapper::set_border)
	.method("perform_voronoi", &zoning_wrapper::perform_voronoi)
	.method("release_voronoi", &zoning_wrapper::release_voronoi)
	.method("get_voronoi_map", &zoning_wrapper::get_voronoi_map)
	.method("get_bounded_feature_size", &zoning_wrapper::get_bounded_feature_size)
	.method("set_all_neighborhood", &zoning_wrapper::set_all_neighborhood)
	.method("set_edge_length_neighborhood", &zoning_wrapper::set_edge_length_neighborhood)
	.method("perform_neighborhood", &zoning_wrapper::perform_neighborhood)
	.method("release_neighborhood", &zoning_wrapper::release_neighborhood)
	.method("get_neighborhood_map", &zoning_wrapper::get_neighborhood_map)
	.method("set_attribute_distances", &zoning_wrapper::set_attribute_distances)
	.method("set_zone_distance", static_cast<void (zoning_wrapper::*)(const minimum<double> &)>(&zoning_wrapper::set_zone_distance), "set minimum zone distance", is_minimum_zone_distance)
	.method("set_zone_distance", static_cast<void (zoning_wrapper::*)(const maximum<double> &)>(&zoning_wrapper::set_zone_distance), "set maximum zone distance", is_maximum_zone_distance)
	.method("set_zone_distance", static_cast<void (zoning_wrapper::*)(const util::mean<double> &)>(&zoning_wrapper::set_zone_distance), "set mean zone distance", is_mean_zone_distance)
	.method("set_combine_distance", static_cast<void (zoning_wrapper::*)(const euclidean_distance<double> &)>(&zoning_wrapper::set_multidimensional_distance), "set euclidean multidimensional distance", is_euclidean_combine_distance)
	.method("set_combine_distance", static_cast<void (zoning_wrapper::*)(const minkowski_distance<double> &)>(&zoning_wrapper::set_multidimensional_distance), "set minkowski multidimensional distance", is_minkowski_combine_distance)
	.method("perform_fusion", &zoning_wrapper::perform_fusion)
	.method("release_fusion", &zoning_wrapper::release_fusion)
	.method("get_fusion_size", &zoning_wrapper::get_fusion_size)
	.method("set_merge", static_cast<void (zoning_wrapper::*)(const size_merge &)>(&zoning_wrapper::set_merge), "set size merge", is_size_merge)
	.method("set_merge", static_cast<void (zoning_wrapper::*)(const area_merge &)>(&zoning_wrapper::set_merge), "set area merge", is_area_merge)
	.method("perform_merge", &zoning_wrapper::perform_merge)
	.method("release_merge", &zoning_wrapper::release_merge)
	.method("get_merge_size", &zoning_wrapper::get_merge_size)
	.method("get_merge_map", &zoning_wrapper::get_merge_map)
	.method("get_merge_maps", &zoning_wrapper::get_merge_maps);

	class_<minimum<double> >("minimum_wrapper")
	.constructor();

	class_<maximum<double> >("maximum_wrapper")
	.constructor();

	class_<util::mean<double> >("mean_wrapper")
	.constructor();

	class_<euclidean_distance<double> >("euclidean_wrapper")
	.constructor();

	class_<minkowski_distance<double> >("minkowski_wrapper")
	.constructor<double>();

	class_<fuzzy_distance>("fuzzy_wrapper")
	.constructor<const fisin_wrapper &>();

	class_<size_merge>("size_wrapper")
	.constructor<int>();

	class_<area_merge>("area_wrapper")
	.constructor<double>();
}
