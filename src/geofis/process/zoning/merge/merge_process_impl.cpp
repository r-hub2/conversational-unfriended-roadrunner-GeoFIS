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
#include <geofis/process/zoning/merge/merge_process_impl.hpp>
#include <geofis/process/zoning/merge/merge_map_predicate.hpp>
#include <geofis/algorithm/zoning/merging/merging.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighboring_predicate.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/aggregation_adaptor.hpp>
#include <geofis/algorithm/zoning/fusion/distance/zone_distance_adapter.hpp>
#include <util/functional/unary_adaptor.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/iterator/filter_iterator.hpp>

using namespace util;
using namespace boost;
using namespace boost::adaptors;

namespace geofis {

merge_process_impl::merge_process_impl(const fusion_map_range_type &fusion_map_range, const merge_type &merge) : merge_predicate(make_unary_adaptor(merge)) {
	auto reversed_fusion_map_range = fusion_map_range | reversed;
	auto merge_map_predicate = make_merge_map_predicate(merge_predicate);
	auto begin_filter_iterator = make_filter_iterator(merge_map_predicate, boost::begin(reversed_fusion_map_range), boost::end(reversed_fusion_map_range));
	auto end_filter_iterator = make_filter_iterator(merge_map_predicate, boost::end(reversed_fusion_map_range), boost::end(reversed_fusion_map_range));
	fusion_maps.assign(begin_filter_iterator, end_filter_iterator);
	UTIL_ENSURE(fusion_maps.size() >= 1);
}

merge_process_impl::~merge_process_impl() {}

size_t merge_process_impl::get_merge_size() const {
	return fusion_maps.size();
}

merge_process_impl::merge_map_type merge_process_impl::get_merge_map(size_t map_index, const neighborhood_type &neighborhood, const aggregation_type &aggregation, const zone_distance_type &zone_distance, const multidimensional_distance_type &multidimensional_distance, const const_attribute_distance_range_type &attribute_distances) const {

	typedef merge_process_traits::zone_type zone_type;
	typedef merging<zone_type> merging_type;
	typedef binary_adaptor<neighborhood_type> neighborhood_adaptor_type;
	typedef zone_neighboring_predicate<neighborhood_adaptor_type> neighboring_type;
	typedef merge_process_traits::feature_distance_type feature_distance_type;
	typedef zone_distance_adapter<zone_distance_type, feature_distance_type> zone_distance_adapter_type;
	typedef aggregation_adaptor<aggregation_type> aggregation_adaptor_type;

	merging_type merging(merge_predicate, fusion_maps[map_index].get_zones());
	neighboring_type neighboring(neighborhood);
	zone_distance_adapter_type zone_distance_adapter(zone_distance, multidimensional_distance, attribute_distances);
	aggregation_adaptor_type aggregatrion_adaptor(aggregation);
	merging.compute_merge_zones(neighboring, zone_distance_adapter, aggregatrion_adaptor);
	return merge_map_type(merging.get_mergeable_zones());
}

} // namespace geofis
