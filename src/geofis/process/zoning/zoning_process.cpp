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
#include <geofis/process/zoning/zoning_process.hpp>
#include <geofis/process/zoning/zoning_process_impl.hpp>

using namespace boost::adaptors;

namespace geofis {

zoning_process::zoning_process(const feature_container_type &features) : impl(new zoning_process_impl(features)) {}

zoning_process::~zoning_process() {}

void zoning_process::set_border(const polygon_type &border) {
	impl->set_border(border);
}

zoning_process::polygon_type zoning_process::get_border() const {
	return impl->get_border();
}

void zoning_process::compute_voronoi_process() {
	impl->compute_voronoi_process();
}

void zoning_process::release_voronoi_process() {
	impl->release_voronoi_process();
}

bool zoning_process::is_voronoi_implemented() const {
	return impl->is_voronoi_implemented();
}

const zoning_process::voronoi_map_type &zoning_process::get_voronoi_map() const {
	return impl->get_voronoi_map();
}

void zoning_process::set_neighborhood(const neighborhood_type &neighborhood) {
	impl->set_neighborhood(neighborhood);
}

void zoning_process::compute_neighborhood_process() {
	impl->compute_neighborhood_process();
}

void zoning_process::release_neighborhood_process() {
	impl->release_neighborhood_process();
}

bool zoning_process::is_neighborhood_implemented() const {
	return impl->is_neighborhood_implemented();
}

zoning_process::const_zone_neighbor_range_type zoning_process::get_zone_neighbors() const {
	return impl->get_zone_neighbors();
}

zoning_process::const_zone_neighbor_range_type zoning_process::get_filtered_zone_neighbors() const {
	return impl->get_filtered_zone_neighbors();
}

void zoning_process::set_aggregation(const aggregation_type &aggregation) {
	impl->set_aggregation(aggregation);
}

void zoning_process::set_zone_distance(const zone_distance_type &zone_distance) {
	impl->set_zone_distance(zone_distance);
}

void zoning_process::set_multidimensional_distance(const multidimensional_distance_type &multidimensional_distance) {
	impl->set_multidimensional_distance(multidimensional_distance);
}

void zoning_process::set_attribute_distances(const attribute_distance_container_type &attribute_distances) {
	impl->set_attribute_distances(attribute_distances);
}

void zoning_process::compute_fusion_process() {
	impl->compute_fusion_process();
}

void zoning_process::release_fusion_process() {
	impl->release_fusion_process();
}

bool zoning_process::is_fusion_implemented() const {
	return impl->is_fusion_implemented();
}

size_t zoning_process::get_fusion_size() const {
	return impl->get_fusion_size();
}

zoning_process::fusion_map_range_type zoning_process::get_fusion_maps(size_t begin, size_t end, bool compute_zones) {
	return impl->get_fusion_maps(begin, end, compute_zones);
}

zoning_process::reverse_fusion_map_range_type zoning_process::get_reverse_fusion_maps(size_t begin, size_t end, bool compute_zones) {
	return reverse(impl->get_fusion_maps(begin, end, compute_zones));
}

size_t zoning_process::get_unique_feature_size() const {
	return impl->get_unique_feature_size();
}

size_t zoning_process::get_bounded_feature_size() const {
	return impl->get_bounded_feature_size();
}

void zoning_process::set_merge(const merge_type &merge) {
	impl->set_merge(merge);
}

zoning_process::merge_type zoning_process::get_merge() const {
	return impl->get_merge();
}

void zoning_process::compute_merge_process() {
	impl->compute_merge_process();
}

void zoning_process::release_merge_process() {
	impl->release_merge_process();
}

bool zoning_process::is_merge_implemented() const {
	return impl->is_merge_implemented();
}

size_t zoning_process::get_merge_size() const {
	return impl->get_merge_size();
}

zoning_process::merge_map_type zoning_process::get_merge_map(size_t map_index) const {
	return impl->get_merge_map(map_index);
}

} // namespace geofis
