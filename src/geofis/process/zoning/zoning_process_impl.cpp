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
#include <geofis/process/zoning/zoning_process_impl.hpp>
#include <boost/range/algorithm/stable_sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <geofis/geometry/polygon.hpp>
#include <geofis/geometry/geometrical_comparator.hpp>
#include <geofis/geometry/geometrical_equal.hpp>
#include <geofis/geometry/feature_bounded.hpp>
#include <geofis/identifiable/identifiable_comparator.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/aggregation_adaptor.hpp>

using namespace util;
using namespace boost;

namespace geofis {

zoning_process_impl::zoning_process_impl(const feature_container_type &features) : features(features) {
	initialize_features();
}

zoning_process_impl::~zoning_process_impl() {}

void zoning_process_impl::initialize_features() {

	typedef zoning_process_traits::feature_type feature_type;

	stable_sort(features, geometrical_comparator<feature_type>());
	unique_features = unique<return_begin_found>(features, geometrical_equal());
}

void zoning_process_impl::set_border(const polygon_type &border) {
	UTIL_REQUIRE(is_valid_polygon(border));
	this->border = border;
	bounded_features = stable_partition<return_begin_found>(unique_features, make_feature_bounded(border));
	sort(bounded_features, identifiable_comparator());
}

zoning_process_impl::polygon_type zoning_process_impl::get_border() const {
	return border;
}

void zoning_process_impl::compute_voronoi_process() {
	voronoi_process_type _voronoi_process(bounded_features, border);
	this->_voronoi_process = boost::move(_voronoi_process);
}

void zoning_process_impl::release_voronoi_process() {
	voronoi_process_type _voronoi_process;
	this->_voronoi_process = boost::move(_voronoi_process);
}

bool zoning_process_impl::is_voronoi_implemented() const {
	return this->_voronoi_process.is_implemented();
}

const zoning_process_impl::voronoi_map_type &zoning_process_impl::get_voronoi_map() const {
	return _voronoi_process.get_voronoi_map();
}

void zoning_process_impl::set_neighborhood(const neighborhood_type &neighborhood) {
	this->neighborhood = neighborhood;
}

void zoning_process_impl::compute_neighborhood_process() {
	neighborhood_process_type _neighborhood_process(neighborhood, _voronoi_process.get_finite_edges());
	this->_neighborhood_process = boost::move(_neighborhood_process);
}

void zoning_process_impl::release_neighborhood_process() {
	neighborhood_process_type _neighborhood_process;
	this->_neighborhood_process = boost::move(_neighborhood_process);
}

bool zoning_process_impl::is_neighborhood_implemented() const {
	return this->_neighborhood_process.is_implemented();
}

zoning_process_impl::const_zone_neighbor_range_type zoning_process_impl::get_zone_neighbors() const {
	return _neighborhood_process.get_zone_neighbors();
}

zoning_process_impl::const_zone_neighbor_range_type zoning_process_impl::get_filtered_zone_neighbors() const {
	return _neighborhood_process.get_filtered_zone_neighbors();
}

void zoning_process_impl::set_aggregation(const aggregation_type &aggregation) {
	this->aggregation = aggregation;
}

void zoning_process_impl::set_zone_distance(const zone_distance_type &zone_distance) {
	this->zone_distance = zone_distance;
}

void zoning_process_impl::set_multidimensional_distance(const multidimensional_distance_type &multidimensional_distance) {
	this->multidimensional_distance = multidimensional_distance;
}

void zoning_process_impl::set_attribute_distances(const attribute_distance_container_type &attribute_distances) {
	this->attribute_distances = attribute_distances;
}

void zoning_process_impl::compute_fusion_process() {
	fusion_process_type _fusion_process(aggregation, zone_distance, multidimensional_distance, attribute_distances, bounded_features, _neighborhood_process.get_zone_neighbors());
	this->_fusion_process = boost::move(_fusion_process);
}

void zoning_process_impl::release_fusion_process() {
	fusion_process_type _fusion_process;
	this->_fusion_process = boost::move(_fusion_process);
}

bool zoning_process_impl::is_fusion_implemented() const {
	return this->_fusion_process.is_implemented();
}

size_t zoning_process_impl::get_fusion_size() const {
	return _fusion_process.get_fusion_size();
}

zoning_process_impl::fusion_map_range_type zoning_process_impl::get_fusion_maps(size_t begin, size_t end, bool compute_zones) {
	return _fusion_process.get_fusion_maps(_voronoi_process.get_zones(), begin, end, compute_zones);
}

size_t zoning_process_impl::get_unique_feature_size() const {
	return distance(unique_features);
}

size_t zoning_process_impl::get_bounded_feature_size() const {
	return distance(bounded_features);
}

void zoning_process_impl::set_merge(const merge_type &merge) {
	this->merge = merge;
}

zoning_process_impl::merge_type zoning_process_impl::get_merge() const {
	return merge;
}

void zoning_process_impl::compute_merge_process() {
	size_t fusion_size = _fusion_process.get_fusion_size();
	fusion_map_range_type fusion_map_range = _fusion_process.get_fusion_maps(_voronoi_process.get_zones(), 0, fusion_size, false);
	merge_process_type _merge_process(fusion_map_range, merge);
	this->_merge_process = boost::move(_merge_process);
}

void zoning_process_impl::release_merge_process() {
	merge_process_type _merge_process;
	this->_merge_process = boost::move(_merge_process);
}

bool zoning_process_impl::is_merge_implemented() const {
	return this->_merge_process.is_implemented();
}

size_t zoning_process_impl::get_merge_size() const {
	return _merge_process.get_merge_size();
}

zoning_process_impl::merge_map_type zoning_process_impl::get_merge_map(size_t map_index) const {
	return _merge_process.get_merge_map(map_index, neighborhood, aggregation, zone_distance, multidimensional_distance, attribute_distances);
}

} // namespace geofis
