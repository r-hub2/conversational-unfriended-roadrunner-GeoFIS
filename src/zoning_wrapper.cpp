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
#include <zoning_wrapper.h>
#include <vector>
#include <boost/format.hpp>
#include <boost/icl/closed_interval.hpp>
#include <boost/icl/continuous_interval.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <rcpp/vector_range.hpp>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/process/zoning/zoning_process.hpp>
#include <geofis/rcpp/geometry/polygon_2.hpp>
#include <geofis/rcpp/geometry/polygons.hpp>
#include <geofis/rcpp/feature/feature_range.hpp>
#include <geofis/rcpp/process/zoning/voronoi/voronoi_map.hpp>
#include <geofis/rcpp/process/zoning/neighborhood/neighborhood_map.hpp>
#include <geofis/rcpp/process/zoning/map.hpp>

using namespace Rcpp;
using namespace std;
using namespace boost;
using namespace util;
using namespace fispro;
using namespace geofis;
using namespace boost::icl;
using namespace boost::adaptors;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef zoning_process_traits::point_type point_type;
typedef zoning_process_traits::polygon_type polygon_type;

typedef zoning_process_traits::kernel_type kernel_type;
typedef zoning_process_traits::feature_type feature_type;
typedef zoning_process_traits::attribute_distance_type attribute_distance_type;
typedef zoning_process_traits::attribute_distance_container_type attribute_distance_container_type;
typedef zoning_process_traits::fusion_map_type fusion_map_type;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RCPP_EXPOSED_AS(euclidean_distance<double>)

RCPP_EXPOSED_AS(minkowski_distance<double>)

RCPP_EXPOSED_AS(fuzzy_distance)

struct attribute_distance_maker {

	typedef attribute_distance_type result_type;

	result_type operator()(SEXP sexp) const {
		if(Rf_isS4(sexp)) {
			S4 s4 = sexp;
			if(s4.is("Rcpp_euclidean_wrapper")) {
				return as<euclidean_distance<double> >(sexp);
			}
			else if(s4.is("Rcpp_fuzzy_wrapper"))
				return as<fuzzy_distance>(sexp);
			else
				stop("unsupported attribute distance");
		} else if(Rf_isNull(sexp)) {
			return none_distance<double>();
		} else
			stop("unsupported attribute distance");
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct merge_checker : public static_visitor<void> {

	const ::zoning_wrapper &zoning_wrapper;

	merge_checker(const ::zoning_wrapper &zoning_wrapper) : zoning_wrapper(zoning_wrapper) {}

    void operator ()(const size_merge &size_merge) const {
    	zoning_wrapper.check_size_merge(size_merge);
    }

    void operator ()(const area_merge &area_merge) const {
    	zoning_wrapper.check_area_merge(area_merge);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

zoning_wrapper::zoning_wrapper(S4 source) : source(source) {
	NumericMatrix points = source.slot("coords");
	DataFrame data_frame = wrap(source.slot("data"));
	Function as_matrix("as.matrix");
	NumericMatrix data = as_matrix(source.slot("data"));
	Function row_names("rownames");
	CharacterVector ids = row_names(data_frame);
	auto feature_range = make_rcpp_feature_range<feature_type>(ids, points, data);
	vector<feature_type> features(boost::begin(feature_range), boost::end(feature_range));
	zp.reset(new zoning_process(features));
}

zoning_wrapper::~zoning_wrapper() {}

void zoning_wrapper::set_border(S4 border) {
	List polygons_list = border.slot("polygons");
	S4 polygons = polygons_list[0];
	List polygon_list = polygons.slot("Polygons");
	zp->set_border(make_polygon_2<kernel_type>(polygon_list[0]));
}

void zoning_wrapper::perform_voronoi() {
	if(!zp->is_voronoi_implemented())
		zp->compute_voronoi_process();
}

void zoning_wrapper::release_voronoi() {
	zp->release_voronoi_process();
}

Nullable<S4> zoning_wrapper::get_voronoi_map() {
	if(zp->is_voronoi_implemented())
		return make_rcpp_voronoi_map(zp->get_voronoi_map(), source.slot("proj4string"));
	else
		return R_NilValue;
}

size_t zoning_wrapper::get_bounded_feature_size() const {
	return zp->get_bounded_feature_size();
}

void zoning_wrapper::set_all_neighborhood() {
	zp->set_neighborhood(all_neighbors());
}

void zoning_wrapper::set_edge_length_neighborhood(double edge_length) {
	zp->set_neighborhood(edge_length_neighborhood(edge_length));
}

void zoning_wrapper::perform_neighborhood() {
	if(!zp->is_neighborhood_implemented())
		zp->compute_neighborhood_process();
}

void zoning_wrapper::release_neighborhood() {
	zp->release_neighborhood_process();
}

Nullable<Rcpp::S4> zoning_wrapper::get_neighborhood_map() {
	if(zp->is_neighborhood_implemented())
		return make_rcpp_neighborhood_map(zp->get_zone_neighbors(), zp->get_filtered_zone_neighbors(), source.slot("proj4string"));
	else
		return R_NilValue;
}

void zoning_wrapper::set_attribute_distances(List attribute_distance_list) {
	auto attribute_distance_range = make_vector_range(attribute_distance_list) | transformed(attribute_distance_maker());
	attribute_distance_container_type attribute_distances(boost::begin(attribute_distance_range), boost::end(attribute_distance_range));
	zp->set_attribute_distances(attribute_distances);
}

void zoning_wrapper::set_zone_distance(const maximum<double> &maximum_distance) {
	zp->set_zone_distance(maximum_distance);
}

void zoning_wrapper::set_zone_distance(const minimum<double> &minimum_distance) {
	zp->set_zone_distance(minimum_distance);
}

void zoning_wrapper::set_zone_distance(const util::mean<double> &mean_distance) {
	zp->set_zone_distance(mean_distance);
}

void zoning_wrapper::set_multidimensional_distance(const euclidean_distance<double> &euclidean_distance) {
	zp->set_multidimensional_distance(euclidean_distance);
}

void zoning_wrapper::set_multidimensional_distance(const minkowski_distance<double> &minkowski_distance) {
	zp->set_multidimensional_distance(minkowski_distance);
}

void zoning_wrapper::perform_fusion() {
	if(!zp->is_fusion_implemented())
		zp->compute_fusion_process();
}

void zoning_wrapper::release_fusion() {
	zp->release_fusion_process();
}

// must return int instead of size_t (return type of zp->get_fusion_size), otherwise NA_INTEGER is not handled as NA in R
int zoning_wrapper::get_fusion_size () {
	return zp->is_fusion_implemented() ? zp->get_fusion_size() : NA_INTEGER;
}

void zoning_wrapper::check_size_merge(const size_merge &size_merge) const {
	Function nrow("nrow");
	int max_size = as<int>(nrow(source));
	closed_interval<size_t> size_interval(1, max_size);
	if(!contains(size_interval, size_merge.size))
		stop(str(format("smallest zone size must be in range %1%") % size_interval));
}

void zoning_wrapper::set_merge(const size_merge &size_merge) {
	check_size_merge(size_merge);
	zp->set_merge(size_merge);
}

void zoning_wrapper::check_area_merge(const area_merge &area_merge) const {
	polygon_type border = zp->get_border();
	double max_area = CGAL::to_double(border.area());
	auto area_interval = continuous_interval<double>::closed(0, max_area);
	if(!contains(area_interval, area_merge.area))
		stop(str(format("smallest zone area must be in range %1%") % area_interval));
}

void zoning_wrapper::set_merge(const area_merge &area_merge) {
	check_area_merge(area_merge);
	zp->set_merge(area_merge);
}

void zoning_wrapper::check_merge(const merge_type &merge) const {
	apply_visitor(merge_checker(*this), merge);
}

void zoning_wrapper::perform_merge() {
	if(!zp->is_merge_implemented()) {
		check_merge(zp->get_merge());
		zp->compute_merge_process();
	}
}

void zoning_wrapper::release_merge() {
	zp->release_merge_process();
}

// must return int instead of size_t (return type of zp->get_merge_size), otherwise NA_INTEGER is not handled as NA in R
int zoning_wrapper::get_merge_size() {
	return zp->is_merge_implemented() ? zp->get_merge_size() : NA_INTEGER;
}

Nullable<S4> zoning_wrapper::get_merge_map(size_t number_of_zones) {
	if(zp->is_merge_implemented()) {
		size_t merge_size = zp->get_merge_size();
		closed_interval<size_t> merge_interval(1, merge_size);
		if(!contains(merge_interval, number_of_zones))
			stop(str(format("number_of_zones must be in range %1%") % merge_interval));
		auto merge_map = zp->get_merge_map(number_of_zones - 1);
		Function col_names("colnames");
		return make_rcpp_map(merge_map, source.slot("proj4string"), col_names(source.slot("data")));
	} else
		return R_NilValue;
}

Nullable<List> zoning_wrapper::get_merge_maps(IntegerVector number_of_zones) {
	if(zp->is_merge_implemented()) {
		auto merge_map_range = number_of_zones | transformed([this](int n) { return this->get_merge_map(n); });
		return List(boost::begin(merge_map_range), boost::end(merge_map_range));
	} else
		return R_NilValue;
}
