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
#ifndef H74C3F1C1_0BB9_4DE7_A1FE_693F42F7D120
#define H74C3F1C1_0BB9_4DE7_A1FE_693F42F7D120

#include <utility>
#include <boost/ref.hpp>
#include <boost/variant.hpp>

namespace geofis {

/**
@startuml

title zone_pair_distance_updater class diagram\n

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface FeatureDistance {
}

hide FeatureDistance members

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class zone_pair_distance_updater<FeatureDistance> {
	+ operator()(value: std::pair<ZonePairIterator, VariantValue>) : void
}

hide zone_pair_distance_updater members
show zone_pair_distance_updater methods

zone_pair_distance_updater o-- "feature_distance" FeatureDistance

note right of zone_pair_distance_updater::operator()
  std::pair<ZonePairIterator, VariantValue> is std::map value_type
  see update_zone_pairs documentation
end note

@enduml
*/

template <class FeatureDistance> class zone_pair_distance_updater {

public:
	zone_pair_distance_updater(const FeatureDistance &feature_distance) : feature_distance(feature_distance) {}

	template <class ZonePairIterator, class VariantValue> void operator()(const std::pair<ZonePairIterator, VariantValue> &value) const {
		update_zone_pair_distance(*value.first, value.second);
	}

private:
	const FeatureDistance &feature_distance;

	template <class ZonePair> struct value_visitor : public boost::static_visitor<void> {

		value_visitor(const FeatureDistance& feature_distance, ZonePair &zone_pair) : feature_distance(feature_distance), zone_pair(zone_pair) {}

		void update_zone_pair_distance_with_duplicate(const ZonePair &duplicate_zone_pair) const {
			zone_pair.update_zone_pair_distance(duplicate_zone_pair.get_zone_pair_distance());
		}

		template <class Zone> void update_zone_pair_distance_with_zones(const Zone &zone1, const Zone &zone2) const {
			zone_pair.update_feature_distances(make_zone_pair_feature_distance(zone1, zone2, feature_distance));
		}

	    template <class ZonePairIterator> result_type operator()(const ZonePairIterator &zone_pair_iterator) const {
	    	update_zone_pair_distance_with_duplicate(*zone_pair_iterator);
	    }

	    template <class ZoneReference> result_type operator()(const std::pair<ZoneReference, ZoneReference> &zone_pair) const {
	    	update_zone_pair_distance_with_zones(boost::unwrap_ref(zone_pair.first), boost::unwrap_ref(zone_pair.second));
	    }

		util::binary_reference_adaptor<const FeatureDistance> feature_distance;
		ZonePair &zone_pair;
	};

	template <class ZonePair, class VariantValue> void update_zone_pair_distance(ZonePair &zone_pair, const VariantValue &value) const {
		boost::apply_visitor(value_visitor<ZonePair>(feature_distance, zone_pair), value);
	}
};

} // namespace geofis

#endif // H74C3F1C1_0BB9_4DE7_A1FE_693F42F7D120 
