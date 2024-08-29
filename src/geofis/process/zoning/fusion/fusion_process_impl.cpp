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
#include <geofis/process/zoning/fusion/fusion_process_impl.hpp>
#include <boost/assign/list_inserter.hpp>
#include <util/range/ref_range.hpp>

using namespace util;
using namespace boost;
using namespace boost::assign;
using namespace boost::adaptors;

namespace geofis {

fusion_process_impl::fusion_process_impl() {}

fusion_process_impl::fusion_process_impl(const aggregation_type &aggregation, const zone_distance_type &zone_distance, const multidimensional_distance_type &multidimensional_distance, attribute_distance_range_type attribute_distances, feature_range_type &features, zone_neighbor_range_type zone_neighbors) : aggregation(aggregation) {
	normalize_attribute_distances(attribute_distances);
	feature_distance = make_feature_distance<feature_distance_type>(multidimensional_distance, attribute_distances);
	feature_normalization_type feature_normalization(feature_normalization_type::initialize(features));
	feature_normalization.normalize(features);
	initialize_zone_pairs_with_neighbors(zone_distance, zone_neighbors);
	aggregate_zone_pairs(zone_pair_updater_type(feature_distance));
}

fusion_process_impl::~fusion_process_impl() {}

struct normalize_attribute_distance {

	struct attribute_distance_normalizer : boost::static_visitor<> {

		template <class AttributeDistance> void operator() (AttributeDistance &) const {}

		void operator() (fispro::fuzzy_distance &fuzzy_distance) const {
			fuzzy_distance.normalize();
		}
	};

	template <BOOST_VARIANT_ENUM_PARAMS(class T)> void operator() (boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &attribute_distance) {
		boost::apply_visitor(attribute_distance_normalizer(), attribute_distance);
	}
};

void fusion_process_impl::normalize_attribute_distances(attribute_distance_range_type &attribute_distances) {
	for_each(attribute_distances, normalize_attribute_distance());
}

void fusion_process_impl::initialize_zone_pairs_with_neighbors(const zone_distance_type &zone_distance, zone_neighbor_range_type &zone_neighbors) {
	push_back(zone_pairs).range(transform(zone_neighbors, neighbor_to_zone_pair_type(zone_distance, feature_distance)));
}

/**
@startuml

title aggregate_zone_pairs activity diagram\n

start

floating note left: In this diagram **zone_pair** and **zone_pair_to_merge** are an iterator type.

:transform **zone_neighbors** to **zone_pairs**;
:sort **zone_pairs** by id comparator;
note right
	this guarantees algorithm reproducibility
	CGAL delaunay triangulation is not reproducible
	so **zone_neighbors** are not ordered

	**zone_pairs** is std::list<zone_pair>
	this guarantees iterator validity on remove operation
end note

while(**zone_pairs** empty ?) is (false)
	:copy all **zone_pair** to merge in **zone_pairs_to_merge**
	according the **aggregation** function;
	note right
		the **aggregation** function must guarantees that
		**zone_pairs** are ordered by merging order and
		**zone_pairs_to_merge** have same merging order

		**zone_pairs_to_merge** is std::list<**zone_pair**>
		this guarantees iterator validity on remove operation
	end note

	:**zone_pair_to_merge** first element of **zone_pairs_to_merge**
	build **zone_fusion** with **zone_pair_to_merge**
	copy **zone_fusion** in **zone_fusions**;

	:remove **zone_pair_to_merge** in **zone_pairs_to_merge**
	remove **zone_pair_to_merge** in **zone_pairs**;

	:update all **zone_pairs** according **zone_pair_updater** and **zone_fusion**;
end while (true)

end

@enduml
*/

struct print_zone_pair_size {

	template <class ZonePairIterator> void operator()(ZonePairIterator zone_pair_iterator) const {
		std::cout << "zone pair - " << zone_pair_iterator->get_zone1().size() << " - " << zone_pair_iterator->get_zone2().size() << std::endl;
	}
};

/**
 * Compare 2 zone_pairs according to their maximum size
 */
struct zone_pair_max_size_comparator {

	template <class ZonePairIterator> bool operator()(ZonePairIterator lhs, ZonePairIterator rhs) const {
		size_t max_size_lhs = std::max(lhs->get_zone1().size(), lhs->get_zone2().size());
		size_t max_size_rhs = std::max(rhs->get_zone1().size(), rhs->get_zone2().size());
		return max_size_lhs > max_size_rhs;
	}
};

/**
 * Compare 2 zone_pairs according to their resulting size after fusion
 */
struct zone_pair_size_comparator {

	template <class ZonePairIterator> bool operator()(ZonePairIterator lhs, ZonePairIterator rhs) const {
		size_t size_lhs = lhs->get_zone1().size() + lhs->get_zone2().size();
		size_t size_rhs = rhs->get_zone1().size() + rhs->get_zone2().size();
		return size_lhs > size_rhs;
	}
};

void fusion_process_impl::aggregate_zone_pairs(const zone_pair_updater_type &zone_pair_updater) {
    zone_pairs.sort(zone_pair_id_comparator());
	while(!zone_pairs.empty()) {
		zone_pair_iterator_container_type zone_pairs_to_merge;
		aggregation(zone_pairs, std::back_inserter(zone_pairs_to_merge));
		// try to sorting zone_pairs_to_merge according a comparison function.
		// not retained: difficult to compare resulting maps.
		//if(zone_pairs_to_merge.size() > 1) {
			//std::cout << zone_pairs_to_merge.size() << " zone_pairs_to_merge" << std::endl;
			//zone_pairs_to_merge.sort(zone_pair_size_comparator());
			//for_each(zone_pairs_to_merge, print_zone_pair_size());
		//}
		aggregate_zone_pair(zone_pairs_to_merge.front(), zone_pair_updater, zone_pairs_to_merge);
	}
}

void fusion_process_impl::aggregate_zone_pair(zone_pair_iterator_type zone_pair_to_merge, const zone_pair_updater_type &zone_pair_updater, zone_pair_iterator_container_type &zone_pairs_to_merge) {
	zone_fusions.push_back(zone_fusion_type(*zone_pair_to_merge));
	zone_pairs_to_merge.remove(zone_pair_to_merge);
	zone_pairs.erase(zone_pair_to_merge);
	zone_pair_updater.update_zone_pairs(zone_pairs, zone_fusions.back(), zone_pairs_to_merge);
}

size_t fusion_process_impl::get_fusion_size() const {
	return zone_fusions.size();
}

fusion_process_impl::fusion_map_range_type fusion_process_impl::get_fusion_maps(zone_info_policy_type &zones, size_t begin, size_t end, bool compute_zones) {
	return make_fusion_map_range(zone_fusions, begin, end, make_ref_range(zones), compute_zones);
}

} // namespace geofis
