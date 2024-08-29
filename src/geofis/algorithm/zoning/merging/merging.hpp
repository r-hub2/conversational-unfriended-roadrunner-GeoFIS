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
#ifndef H8055C58E_519A_4BBF_9A55_2C9F769C5A31
#define H8055C58E_519A_4BBF_9A55_2C9F769C5A31

#include <geofis/algorithm/zoning/merging/pair/zone_pair_maker.hpp>
#include <boost/algorithm/cxx11/partition_copy.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/bind/bind.hpp>
#include <util/iterator/output/back_insert_reference_iterator.hpp>
#include <util/range/algorithm/copy_if.hpp>
#include <util/assert.hpp>
#include <list>

namespace geofis {

/*
@startuml

title merging class diagram\n

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class Zone {
	+ merge(zone: Zone)
}

hide Zone members
show Zone methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface MergeablePredicate {
	+ operator()(zone: Zone): bool
}

hide MergeablePredicate members
show MergeablePredicate methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZoneNeighborPredicate {
	+ operator()(lhs: Zone, rhs: Zone): bool
}

hide ZoneNeighborPredicate members
show ZoneNeighborPredicate methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZoneDistance {
	+ operator()(lhs: Zone, rhs: Zone): double
}

hide ZoneDistance members
show ZoneDistance methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class zone_pair<Zone>

hide zone_pair members

zone_pair ..> ZoneDistance : <<call>>

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZonePairMerger {
	+ operator()(zone_pairs: ZonePairRange, Out zone_pair_to_merge: ZonePairIterator)
}

hide ZonePairMerger members
show ZonePairMerger methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class merging<Zone> {
	- mergeable_zones: Zone [0..*]
	- merged_zones: Zone [0..*]
	+ «constructor» merging(mergeable_predicate: MergeablePredicate, zones: ZoneRange)
	+ compute_merge_zones(zone_neighbor_predicate: ZoneNeighborPredicate, zone_distance: ZoneDistance, zone_pair_merger: ZonePairMerger)
	+ get_mergeable_zones(): const_zone_range_type
	+ get_merged_zones(): const_zone_range_type
	+ get_mergeable_zones_size(): size_t
	+ get_merged_zones_size(): size_t
	+ is_mergeable_zones_empty(): bool
	+ is_merged_zones_empty(): bool
}

merging .> Zone : <<use>>
merging ..> MergeablePredicate : <<call>>
merging ..> ZoneNeighborPredicate : <<call>>
merging ..> ZonePairMerger : <<call>>
merging ..> zone_pair : <<create>>

@enduml
*/

/*
@startuml

title merging algorithm activity diagram\n

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

start

:partition incoming **zones** in **mergeable_zones** and **merged_zones** \nbased on **mergeable_predicate**;
note right
	**mergeable_zones** satisfies the **mergeable_predicate**
end note

while (**merged_zones** empty ?) is (false)
	while (for each **merged_zone** in **merged_zones**)
    	:copy all **mergeable_zone** neighbor of **merged_zone** in **zone_neighbors**\nbased on **zone_neighbor_predicate**;
    	if (**zone_neighbors** empty ?) then (false)
      		if (**zone_neighbors** size == 1 ?) then (true)
        		:merge **merged_zone** inside **zone_neighbor**;
      	  	else (false)
        		:transform **zone_neighbors** to **zone_pairs**\ncomputing distance between **merged_zone** and **zone_neighbor**\nbased on **zone_distance** function;
        		:copy all **zone_pair** to merge in **zone_pairs_to_merge** \nbased on **zone_pair_merger** function;
        		:merge first **zone_pairs_to_merge**;
        		note right
                    at these step if **zone_pairs_to_merge** contains more
                    than 1 zone, we can't determine which **zone_pair** to merge
                    so we merge the first of **zone_pairs_to_merge**
        		end note
      		endif
        	:remove **merged_zone** from **merged_zones**;
    	else (true)
    	endif
	end while
endwhile (true)

stop

@enduml
*/

template <class Zone> class merging {

	typedef std::list<Zone> zone_container_type;
	typedef boost::sub_range<const zone_container_type> const_zone_range_type;
	typedef typename boost::range_iterator<zone_container_type>::type zone_iterator_type;

	typedef zone_pair<Zone> zone_pair_type;
	typedef std::list<zone_pair_type> zone_pair_container_type;
	typedef typename boost::range_iterator<zone_pair_container_type>::type zone_pair_iterator_type;
	typedef std::list<zone_pair_iterator_type> zone_pair_iterator_container_type;

	typedef boost::reference_wrapper<Zone> zone_reference_type;
	typedef std::list<zone_reference_type> zone_reference_container_type;

private:
	zone_container_type mergeable_zones;
	zone_container_type merged_zones;

public:
	merging() {}
	template <class MergeablePredicate, class ZoneRange> merging(const MergeablePredicate &mergeable_predicate, const ZoneRange &zones) {
		partition_zones(zones, mergeable_predicate);
	}

	const_zone_range_type get_merged_zones() const {
		return merged_zones;
	}

	size_t get_merged_zones_size() const {
		return merged_zones.size();
	}

	bool is_merged_zones_empty() const {
		return merged_zones.empty();
	}

	const_zone_range_type get_mergeable_zones() const {
		return mergeable_zones;
	}

	size_t get_mergeable_zones_size() const {
		return mergeable_zones.size();
	}

	bool is_mergeable_zones_empty() const {
		return mergeable_zones.empty();
	}

	template <class ZoneNeighborPredicate, class ZoneDistance, class ZonePairMerger> void compute_merge_zones(const ZoneNeighborPredicate &zone_neighbor_predicate, const ZoneDistance &zone_distance, const ZonePairMerger &zone_pair_merger) {
		while(!merged_zones.empty()) {
#ifndef R_PACKAGE
			size_t merged_zones_size = merged_zones.size();
#endif
			compute_merged_zones(boost::begin(merged_zones), boost::end(merged_zones), zone_neighbor_predicate, zone_distance, zone_pair_merger);
#ifndef R_PACKAGE
			UTIL_ENSURE( merged_zones_size != merged_zones.size() );
#endif
		}
	}

private:
	template <class ZoneRange, class MergeablePredicate> void partition_zones(const ZoneRange &zones, const MergeablePredicate &mergeable_predicate) {
		boost::algorithm::partition_copy(zones, std::back_inserter(mergeable_zones), std::back_inserter(merged_zones), mergeable_predicate);
	}

	template <class ZoneNeighborPredicate, class ZoneDistance, class ZonePairMerger> void compute_merged_zones(zone_iterator_type iterator, zone_iterator_type last_iterator, const ZoneNeighborPredicate &zone_neighbor_predicate, const ZoneDistance &zone_distance, const ZonePairMerger &zone_pair_merger) {
		while(iterator != last_iterator) {
			if(compute_merged_zone(*iterator, zone_neighbor_predicate, zone_distance, zone_pair_merger)) {
				zone_iterator_type erase_iterator = iterator;
				zone_iterator_type next_iterator = ++iterator;
				merged_zones.erase(erase_iterator);
				iterator = next_iterator;
			} else
				++iterator;
		}
	}

	template <class ZoneNeighborPredicate, class ZoneDistance, class ZonePairMerger> bool compute_merged_zone(const Zone &merged_zone, const ZoneNeighborPredicate &zone_neighbor_predicate, const ZoneDistance &zone_distance, const ZonePairMerger &zone_pair_merger) {
		zone_reference_container_type zone_neighbors;
		util::copy_if(mergeable_zones, util::back_inserter_reference(zone_neighbors), boost::bind(zone_neighbor_predicate, boost::placeholders::_1, boost::cref(merged_zone)));
		if(!zone_neighbors.empty()) {
			if(zone_neighbors.size() == 1) {
				boost::unwrap_ref(zone_neighbors.front()).merge(merged_zone);
				return true;
			} else {
				zone_pair_container_type zone_pairs;
				boost::transform(zone_neighbors, std::back_inserter(zone_pairs), zone_pair_maker<Zone, ZoneDistance>(merged_zone, zone_distance));
				zone_pair_iterator_container_type zone_pairs_to_merge;
				zone_pair_merger(zone_pairs, std::back_inserter(zone_pairs_to_merge));
				merge_zone_pair(*zone_pairs_to_merge.front());
				return true;
			}
		}
		return false;
	}

	void merge_zone_pair(const zone_pair_type &zone_pair) {
		zone_pair.get_mergeable_zone().merge(zone_pair.get_merged_zone());
	}
};

} // namespace geofis

#endif // H8055C58E_519A_4BBF_9A55_2C9F769C5A31 
