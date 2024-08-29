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
#ifndef UPDATE_ZONE_PAIR_HPP_
#define UPDATE_ZONE_PAIR_HPP_

#include <map>
#include <list>
#include <vector>
#include <util/address_equal.hpp>
#include <util/functional/binary_reference_adaptor.hpp>
#include <boost/ref.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/range/algorithm/for_each.hpp>
//#include <geofis/algorithm/zoning/zone_pair_equal.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance_updater.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_feature_distance.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_iterator_id_comparator.hpp>

namespace geofis {

/**
@startuml

title zone_pair_updater class diagram\n

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface updated_zone_type <<(T,#FF7700)typedef>>

hide updated_zone_type members

note left: optional reference to updated zone

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface updated_zone_pair_iterator_type <<(T,#FF7700)typedef>>

hide updated_zone_pair_iterator_type members

note right: iterator to std::map\nsee update_zone_pairs documentation

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZonePair {
	+ update_zones(ZoneFusion) : updated_zone_type
}

hide ZonePair members
show ZonePair methods

ZonePair ..> updated_zone_type : <<use>>

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZonePairDistanceUpdater {
	+ operator()(updated_zone_pair_iterator_type): void
}

hide ZonePairDistanceUpdater members
show ZonePairDistanceUpdater methods

ZonePairDistanceUpdater ..> updated_zone_pair_iterator_type : <<use>>

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class zone_pair_updater<ZonePairDistanceUpdater> {
	- zone_pair_distance_updater : ZonePairDistanceUpdater
	+ update_zone_pairs(std::list<ZonePair>, ZoneFusion, std::list<ZonePairIterator>) : void
}

zone_pair_updater ..> ZonePair : <<call>>
zone_pair_updater ..> ZonePairDistanceUpdater : <<call>>

@enduml
*/

template <class ZonePairDistanceUpdater> class zone_pair_updater {

public:
	zone_pair_updater(const ZonePairDistanceUpdater &zone_pair_distance_updater) : zone_pair_distance_updater(zone_pair_distance_updater) {}

	/**
	@startuml

	start

	:update all **zone_pairs** according to **zone_fusion**
	copy duplicate **zone_pairs** in **duplicate_zone_pairs**;
	note left
	 	 **zone_pairs** must be ordered in merging order
	end note

	:remove **duplicate_zone_pairs** from **zone_pairs_to_merge**;
	:remove **duplicate_zone_pairs** from **zone_pairs**;

	end

	@enduml
	*/

	template <class ZonePair, class ZoneFusion, class ZonePairIterator> void update_zone_pairs(std::list<ZonePair> &zone_pairs, ZoneFusion &zone_fusion, std::list<ZonePairIterator> &zone_pairs_to_merge) const {

		typedef typename std::list<ZonePair>::iterator zone_pair_iterator_type;

		std::vector<zone_pair_iterator_type> duplicate_zone_pairs;
		update_zone_pairs(boost::begin(zone_pairs), boost::end(zone_pairs), zone_fusion, std::back_inserter(duplicate_zone_pairs));
		remove_zone_pairs_to_merge(zone_pairs_to_merge, duplicate_zone_pairs);
		remove_zone_pairs(zone_pairs, duplicate_zone_pairs);
	}

private:
	ZonePairDistanceUpdater zone_pair_distance_updater;

	template <class Zone, class ZonePair, class ZoneFusion> std::pair<boost::reference_wrapper<Zone>, boost::reference_wrapper<Zone> > make_zone_pair_to_compute_distance(boost::reference_wrapper<Zone> &updated_zone, ZonePair &zone_pair, ZoneFusion &zone_fusion) const {
		return make_zone_pair_to_compute_distance(boost::unwrap_ref(updated_zone), zone_pair, zone_fusion);
	}

	template <class Zone, class ZonePair> Zone &get_other_zone(Zone &zone, ZonePair &zone_pair) const {
		return util::address_equal(zone, zone_pair.get_zone1()) ? zone_pair.get_zone2() : zone_pair.get_zone1();
	}

	template <class Zone, class ZonePair, class ZoneFusion> std::pair<boost::reference_wrapper<Zone>, boost::reference_wrapper<Zone> > make_zone_pair_to_compute_distance(Zone &updated_zone, ZonePair &zone_pair, ZoneFusion &zone_fusion) const {
		return std::make_pair(boost::ref(get_other_zone(updated_zone, zone_fusion)), boost::ref(get_other_zone(zone_fusion.get_fusion(), zone_pair)));
	}

	/**

	@startuml

	title update_zone_pairs activity diagram\n

	'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

	start

	floating note left: In this diagram **zone_pair** is an iterator type.

	while (for each **zone_pair** in **zone_pairs**)
		:update zones of **zone_pair** according to **zone_fusion**;
		note left
			zone updating replace zone in **zone_pair** with **zone_fusion**

			suppose we have **zone_pairs**: CE, DA, CA, CB
			and **zone_fusion** AB merged in Z
			**zone_pairs** become: CE, DZ, CZ, CZ
			the second CZ **zone_pair** will be treated as **duplicate_zone_pair**
		end note

		if (**zone_pair** updated ?) then (yes)
			:copy **zone_pair** in **updated_zone_pairs**;
			note left
				  **updated_zone_pairs** is a std::map<Key, Value, Compare>

				  * **Key** is a **zone_pair**
				  * **Value** is a variant value to update **zone_pair** distance
				  * **Compare** is **zone_pair** id comparator

				  this guarantees about **zone_pair** uniqueness
			end note

			if (**zone_pair** already present in **updated_zone_pairs** ?) then (yes)
				:copy **zone_pair** in **duplicate_zone_pairs**;
			else (no)
			endif
		else (no)
		endif
	end while

	while (for each **zone_pair** in **updated_zone_pairs**)
		:update **zone_pair** distance;
		note left
			the CA **zone_pair** distance must be updated with CB distance
			* need to compute the CB distance
			* in case of duplicate the CB distance is already computed

			so the the **Value** in std::map **updated_zone_pairs** is variant
			* std::pair<C, B>
			* **zone_pair** CB in case of duplicate
		end note
	end while

	stop

	@enduml
	*/

	template <class ZonePairIterator, class ZoneFusion, class OutputDuplicateZonePair> void update_zone_pairs(ZonePairIterator zone_pair, ZonePairIterator end_zone_pair, ZoneFusion &zone_fusion, OutputDuplicateZonePair duplicate_zone_pairs) const {

		typedef typename ZoneFusion::zone_type zone_type;
		typedef boost::reference_wrapper<zone_type> zone_reference_type;
		typedef std::pair<zone_reference_type, zone_reference_type> zone_pair_reference_type;
		typedef boost::variant<ZonePairIterator, zone_pair_reference_type> value_type;
		typedef std::map<ZonePairIterator, value_type, zone_pair_iterator_id_comparator> updated_zone_pairs_type;
		typedef typename updated_zone_pairs_type::iterator updated_zone_pair_iterator_type;
		typedef boost::optional<zone_reference_type> updated_zone_type;

		updated_zone_pairs_type updated_zone_pairs;
		for(; zone_pair != end_zone_pair; ++zone_pair) {
			updated_zone_type updated_zone = zone_pair->update_zones(zone_fusion);
			if(updated_zone) {
				std::pair<updated_zone_pair_iterator_type, bool> insert_result = updated_zone_pairs.insert(std::make_pair(zone_pair, make_zone_pair_to_compute_distance(boost::get(updated_zone), *zone_pair, zone_fusion)));
				if(!insert_result.second) {
					*duplicate_zone_pairs = zone_pair;
					insert_result.first->second = zone_pair;
				}
			}
		}
		boost::for_each(updated_zone_pairs, zone_pair_distance_updater);
	}

	template <class ZonePairIteratorContainer> struct remove_from_zone_pairs_to_merge {

		remove_from_zone_pairs_to_merge(ZonePairIteratorContainer &zone_pairs_to_merge) : zone_pairs_to_merge(zone_pairs_to_merge) {}

		template <class ZonePairIterator> void operator()(const ZonePairIterator &zone_pair_iterator) const {
			zone_pairs_to_merge.remove(zone_pair_iterator);
		}

		ZonePairIteratorContainer &zone_pairs_to_merge;
	};

	template <class ZonePairIteratorContainer, class DuplicateZonePairRange> void remove_zone_pairs_to_merge(ZonePairIteratorContainer &zone_pairs_to_merge, const DuplicateZonePairRange &duplicate_zone_pairs) const {
		boost::for_each(duplicate_zone_pairs, remove_from_zone_pairs_to_merge<ZonePairIteratorContainer>(zone_pairs_to_merge));
	}

	template <class ZonePairContainer> struct remove_from_zone_pairs {

		remove_from_zone_pairs(ZonePairContainer &zone_pairs) : zone_pairs(zone_pairs) {}

		template <class ZonePairIterator> void operator()(const ZonePairIterator &zone_pair_iterator) const {
			zone_pairs.erase(zone_pair_iterator);
		}

		ZonePairContainer &zone_pairs;
	};

	template <class ZonePairContainer, class DuplicateZonePairRange> void remove_zone_pairs(ZonePairContainer &zone_pairs, const DuplicateZonePairRange &duplicate_zone_pairs) const {
		boost::for_each(duplicate_zone_pairs, remove_from_zone_pairs<ZonePairContainer>(zone_pairs));
	}
};

} // namespace geofis

#endif /* UPDATE_ZONE_PAIR_HPP_ */
