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
#ifndef ZONE_PAIR_HPP_
#define ZONE_PAIR_HPP_

#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <util/address_equal.hpp>

namespace geofis {

/*
@startuml

title zone_pair class diagram

skinparam linetype ortho

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface Zone {
}

hide Zone members

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

interface ZonePairDistance {
	+ get_distance() : double
	+ update(other_zone_pair_distance: ZonePairDistance) : void
	+ update_feature_distances(feature_distances: FeatureDistanceRange)
}

hide ZonePairDistance members
show ZonePairDistance methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

class zone_pair<Zone, ZonePairDistance> {
	- zone_pair_distance : ZonePairDistance
	+ <<getter>> get_zone1() : Zone
	+ <<getter>> get_zone2() : Zone
	+ get_distance() : double
	+ contain_zone(Zone) : bool
}

show zone_pair methods

zone_pair o-- "zone1\nzone2" Zone
zone_pair <.right. zone_pair_updater : <<friend>>
zone_pair <.right. zone_pair_distance_updater : <<friend>>
zone_pair ..> ZonePairDistance : <<call>>

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

hide zone_pair_updater members
hide zone_pair_distance_updater members

@enduml
*/

template <class Zone, class ZonePairDistance> class zone_pair {

	typedef boost::reference_wrapper<Zone> zone_reference_type;
	typedef boost::optional<zone_reference_type> updated_zone_type;

	template <class FeatureDistance> friend class zone_pair_distance_updater;
	template <class ZonePairDistanceUpdater> friend class zone_pair_updater;

public:
	typedef Zone zone_type;
	typedef typename Zone::id_type id_type;

	zone_pair(Zone &zone1, Zone &zone2, const ZonePairDistance &zone_pair_distance) : zone1(zone1), zone2(zone2), zone_pair_distance(zone_pair_distance) {}

	Zone &get_zone1() { return zone1; }
	Zone &get_zone2() { return zone2; }

	const Zone &get_zone1() const { return zone1; }
	const Zone &get_zone2() const { return zone2; }

	double get_distance() const { return zone_pair_distance.get_distance(); }

	bool contain_zone(const Zone &zone) const {
		return util::address_equal(zone, zone1) || util::address_equal(zone, zone2);
	}

private:
	zone_reference_type zone1;
	zone_reference_type zone2;
	ZonePairDistance zone_pair_distance;

	template <class ZoneFusion> updated_zone_type update_zones(ZoneFusion &zone_fusion) {
		if(zone_fusion.contain_zone(zone1))
			return update_zone(zone1, zone_fusion);
		if(zone_fusion.contain_zone(zone2))
			return update_zone(zone2, zone_fusion);
		return updated_zone_type();
	}

	void update_zone_pair_distance(const ZonePairDistance &other_zone_pair_distance) {
		zone_pair_distance.update(other_zone_pair_distance);
	}

	template <class FeatureDistanceRange> void update_feature_distances(const FeatureDistanceRange &feature_distances) {
		zone_pair_distance.update_feature_distances(feature_distances);
	}

	const ZonePairDistance &get_zone_pair_distance() const {
		return zone_pair_distance;
	}

	template <class ZoneFusion> static updated_zone_type update_zone(zone_reference_type &zone, ZoneFusion &zone_fusion) {
		zone_reference_type updated_zone = zone;
		zone = boost::ref(zone_fusion.get_fusion());
		return updated_zone;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone, class ZonePairDistance> inline zone_pair<Zone, ZonePairDistance> make_zone_pair(Zone &zone1, Zone &zone2, const ZonePairDistance &zone_pair_distance) {
	return zone_pair<Zone, ZonePairDistance>(zone1, zone2, zone_pair_distance);
}

} // namespace geofis

#endif /* ZONE_PAIR_HPP_ */
