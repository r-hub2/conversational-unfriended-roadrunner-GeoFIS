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
#ifndef NEIGHBOR_TO_ZONE_PAIR_HPP_
#define NEIGHBOR_TO_ZONE_PAIR_HPP_

#include <boost/ref.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_feature_distance.hpp>
#include <util/functional/binary_reference_adaptor.hpp>

namespace geofis {

template <class Zone, class ZoneDistance, class FeatureDistance> struct neighbor_to_zone_pair {

	typedef zone_pair_distance<ZoneDistance> zone_pair_distance_type;
	typedef zone_pair<Zone, zone_pair_distance_type> result_type;

	neighbor_to_zone_pair(const ZoneDistance &zone_distance, const FeatureDistance &feature_distance) : zone_distance(zone_distance), feature_distance(feature_distance) {}

	result_type make_zone_pair(Zone &zone1, Zone &zone2) const {
		return result_type(zone1, zone2, make_zone_pair_distance(zone_distance, make_zone_pair_feature_distance(zone1, zone2, feature_distance)));
	}

	template <class Neighbor> result_type operator()(Neighbor &neighbor) const {
		return make_zone_pair(neighbor.get_zone1(), neighbor.get_zone2());
	}

	const ZoneDistance &zone_distance;
	util::binary_reference_adaptor<const FeatureDistance> feature_distance;
};

} // namespace geofis

#endif /* NEIGHBOR_TO_ZONE_PAIR_HPP_ */
