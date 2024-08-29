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
#ifndef ZONE_FUSION_HPP_
#define ZONE_FUSION_HPP_

#include <util/address_equal.hpp>
#include <boost/ref.hpp>
#include <boost/range/join.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone_area_computer.hpp>

namespace geofis {

/*
@startuml

title zone_fusion class diagram\n

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
interface Zone {
	+ get_voronoi_zones() : voronoi_zone_range_type
}

hide Zone members
show Zone methods

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
class zone_fusion<Zone> {
	- fusion : Zone
	+ <<constructor>> zone_fusion(zone_pair: ZonePair)
	+ <<getter>> get_zone1(): Zone
	+ <<getter>> get_zone2(): Zone
	+ <<getter>> get_fusion(): Zone
}

zone_fusion o-- "zone1\nzone2" Zone

@enduml
*/

template <class Zone> class zone_fusion {

	typedef typename boost::reference_wrapper<Zone> zone_reference_type;

public:
	typedef Zone zone_type;

	template <class ZonePair> zone_fusion(ZonePair &zone_pair) : zone1(zone_pair.get_zone1()), zone2(zone_pair.get_zone2()), fusion(boost::range::join(zone_pair.get_zone1().get_voronoi_zones(), zone_pair.get_zone2().get_voronoi_zones())) {
		compute_zone_area(fusion, zone1.get(), zone2.get());
	}

	Zone &get_zone1() { return zone1; }
	Zone &get_zone2() { return zone2; }

	const Zone &get_zone1() const { return zone1; }
	const Zone &get_zone2() const { return zone2; }

	Zone &get_fusion() { return fusion; }
	const Zone &get_fusion() const { return fusion; }

	bool contain_zone(const Zone &zone) const {
		return util::address_equal(zone, boost::unwrap_ref(zone1)) || util::address_equal(zone, boost::unwrap_ref(zone2));
	}

private:
	zone_reference_type zone1;
	zone_reference_type zone2;
	Zone fusion;
};

} // namespace geofis

#endif /* ZONE_FUSION_HPP_ */
