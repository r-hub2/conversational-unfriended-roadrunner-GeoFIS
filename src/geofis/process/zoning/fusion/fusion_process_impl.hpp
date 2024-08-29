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
#ifndef H941EF80A_E62F_4855_B5E5_651F5F416D3A
#define H941EF80A_E62F_4855_B5E5_651F5F416D3A

#include <list>
#include <geofis/process/zoning/fusion/fusion_process_traits.hpp>
#include <geofis/algorithm/feature/feature_normalization.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/aggregation_adaptor.hpp>
#include <geofis/algorithm/zoning/fusion/neighbor_to_zone_pair.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance_updater.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_updater.hpp>

namespace geofis {

class fusion_process_impl {

	typedef fusion_process_traits::aggregation_type aggregation_type;
	typedef fusion_process_traits::zone_distance_type zone_distance_type;
	typedef fusion_process_traits::multidimensional_distance_type multidimensional_distance_type;
	typedef fusion_process_traits::attribute_distance_range_type attribute_distance_range_type;
	typedef fusion_process_traits::feature_range_type feature_range_type;
	typedef fusion_process_traits::zone_neighbor_range_type zone_neighbor_range_type;

	typedef aggregation_adaptor<aggregation_type> aggregation_adaptor_type;

	typedef fusion_process_traits::feature_distance_type feature_distance_type;

	typedef fusion_process_traits::feature_type feature_type;
	typedef feature_normalization<feature_type> feature_normalization_type;

	typedef zone_pair_distance<zone_distance_type> zone_pair_distance_type;
	typedef fusion_process_traits::zone_type zone_type;
	typedef zone_pair<zone_type, zone_pair_distance_type> zone_pair_type;
	typedef std::list<zone_pair_type> zone_pair_container_type;
	typedef typename boost::range_iterator<zone_pair_container_type>::type zone_pair_iterator_type;
	typedef std::list<zone_pair_iterator_type> zone_pair_iterator_container_type;
	typedef neighbor_to_zone_pair<zone_type, zone_distance_type, feature_distance_type> neighbor_to_zone_pair_type;

	typedef fusion_process_traits::zone_fusion_type zone_fusion_type;
	typedef fusion_process_traits::zone_fusion_container_type zone_fusion_container_type;

	typedef zone_pair_distance_updater<feature_distance_type> zone_pair_distance_updater_type;
	typedef zone_pair_updater<zone_pair_distance_updater_type> zone_pair_updater_type;

	typedef fusion_process_traits::zone_info_policy_type zone_info_policy_type;
	typedef fusion_process_traits::fusion_map_range_type fusion_map_range_type;

	aggregation_adaptor_type aggregation;
	feature_distance_type feature_distance;
	zone_pair_container_type zone_pairs;
	zone_fusion_container_type zone_fusions;

public:
	fusion_process_impl();
	fusion_process_impl(const aggregation_type &aggregation, const zone_distance_type &zone_distance, const multidimensional_distance_type &multidimensional_distance, attribute_distance_range_type attribute_distances, feature_range_type &features, zone_neighbor_range_type zone_neighbors);
	~fusion_process_impl();

	size_t get_fusion_size() const;
	fusion_map_range_type get_fusion_maps(zone_info_policy_type &zones, size_t begin, size_t end, bool compute_zones = false);

private:
	void initialize_zone_pairs_with_neighbors(const zone_distance_type &zone_distance, zone_neighbor_range_type &zone_neighbors);
	void aggregate_zone_pairs(const zone_pair_updater_type &zone_pair_updater);
	void aggregate_zone_pair(zone_pair_iterator_type zone_pair_to_merge, const zone_pair_updater_type &zone_pair_updater, zone_pair_iterator_container_type &zone_pairs_to_merge);
	void normalize_attribute_distances(attribute_distance_range_type &attribute_distances);
};

} // namespace geofis

#endif // H941EF80A_E62F_4855_B5E5_651F5F416D3A 
