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
#ifndef HE398EA57_9602_4906_A051_BAF8095DD924
#define HE398EA57_9602_4906_A051_BAF8095DD924

#include <geofis/process/zoning/zoning_process_traits.hpp>
#include <geofis/process/zoning/voronoi/voronoi_process.hpp>
#include <geofis/process/zoning/neighborhood/neighborhood_process.hpp>
#include <geofis/process/zoning/fusion/fusion_process.hpp>
#include <geofis/process/zoning/merge/merge_process.hpp>

namespace geofis {

class zoning_process_impl {

	typedef zoning_process_traits::polygon_type polygon_type;
	typedef zoning_process_traits::feature_container_type feature_container_type;
	typedef zoning_process_traits::feature_range_type feature_range_type;
	typedef zoning_process_traits::voronoi_map_type voronoi_map_type;
	typedef zoning_process_traits::neighborhood_type neighborhood_type;
	typedef zoning_process_traits::const_zone_neighbor_range_type const_zone_neighbor_range_type;
	typedef voronoi_process voronoi_process_type;
	typedef neighborhood_process neighborhood_process_type;
	typedef zoning_process_traits::aggregation_type aggregation_type;
	typedef zoning_process_traits::zone_distance_type zone_distance_type;
	typedef zoning_process_traits::multidimensional_distance_type multidimensional_distance_type;
	typedef zoning_process_traits::attribute_distance_type attribute_distance_type;
	typedef zoning_process_traits::attribute_distance_container_type attribute_distance_container_type;
	typedef fusion_process fusion_process_type;
	typedef zoning_process_traits::fusion_map_type fusion_map_type;
	typedef zoning_process_traits::fusion_map_range_type fusion_map_range_type;
	typedef zoning_process_traits::merge_type merge_type;
	typedef zoning_process_traits::merge_map_type merge_map_type;
	typedef merge_process merge_process_type;

	polygon_type border;
	feature_container_type features;
	feature_range_type unique_features;
	feature_range_type bounded_features;
	voronoi_process_type _voronoi_process;
	neighborhood_type neighborhood;
	neighborhood_process_type _neighborhood_process;
	aggregation_type aggregation;
	zone_distance_type zone_distance;
	multidimensional_distance_type multidimensional_distance;
	attribute_distance_container_type attribute_distances;
	fusion_process_type _fusion_process;
	merge_type merge;
	merge_process_type _merge_process;

public:
	zoning_process_impl(const feature_container_type &features);
	template <class FeatureRange> zoning_process_impl(const FeatureRange &features) : features(boost::begin(features), boost::end(features)) {
		initialize_features();
	}

	~zoning_process_impl();

	void set_border(const polygon_type &border);
	polygon_type get_border() const;
	void compute_voronoi_process();
	void release_voronoi_process();
	bool is_voronoi_implemented() const;
	const voronoi_map_type &get_voronoi_map() const;

	size_t get_unique_feature_size() const;
	size_t get_bounded_feature_size() const;

	void set_neighborhood(const neighborhood_type &neighborhood);
	void compute_neighborhood_process();
	void release_neighborhood_process();
	bool is_neighborhood_implemented() const;
	const_zone_neighbor_range_type get_zone_neighbors() const;
	const_zone_neighbor_range_type get_filtered_zone_neighbors() const;

	void set_aggregation(const aggregation_type &aggregation);
	void set_zone_distance(const zone_distance_type &zone_distance);
	void set_multidimensional_distance(const multidimensional_distance_type &multidimensional_distance);
	void set_attribute_distances(const attribute_distance_container_type &attribute_distances);
	void compute_fusion_process();
	void release_fusion_process();
	bool is_fusion_implemented() const;
	size_t get_fusion_size() const;
	fusion_map_range_type get_fusion_maps(size_t begin, size_t end, bool compute_zones);

	void set_merge(const merge_type &merge);
	merge_type get_merge() const;
	void compute_merge_process();
	void release_merge_process();
	bool is_merge_implemented() const;
	size_t get_merge_size() const;
	merge_map_type get_merge_map(size_t map_index) const;

private:
	void initialize_features();
};

} // namespace geofis

#endif // HE398EA57_9602_4906_A051_BAF8095DD924 
