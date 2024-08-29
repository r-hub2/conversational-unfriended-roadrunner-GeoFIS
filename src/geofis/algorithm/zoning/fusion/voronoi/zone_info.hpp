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
#ifndef ZONE_INFO_HPP_
#define ZONE_INFO_HPP_

#include <vector>
#include <util/assert.hpp>
#include <util/range/assign.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone_range.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_info.hpp>
#include <util/range/zipped_with_range.hpp>

namespace geofis {

template <class VoronoiZone> struct zone_info_traits {

	typedef voronoi_info<VoronoiZone> voronoi_info_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone, class VoronoiZone> class zone_info {

	typedef typename zone_info_traits<VoronoiZone>::voronoi_info_type voronoi_info_type;

public:
	zone_info() {}
	zone_info(Zone &zone, VoronoiZone &voronoi_zone) : zone(zone), voronoi_info(voronoi_zone) {}

	template <class Geometry> void set_geometry(const Geometry &geometry) {
		voronoi_info.set_geometry(geometry);
		//zone->set_geometry(geometry);
	}

	Zone &get_zone() {
		return *zone;
	}

	const Zone &get_zone() const {
		return *zone;
	}

	const VoronoiZone &get_voronoi_zone() const {
		return voronoi_info.get_voronoi_zone();
	}

private:
	util::address_wrapper<Zone> zone;
	voronoi_info_type voronoi_info;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneInfo> struct zone_info_builder {

	typedef ZoneInfo result_type;

	template <class Zone, class VoronoiZone> result_type operator()(Zone &zone, VoronoiZone &voronoi_zone) const {
		return result_type(zone, voronoi_zone);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneRange, class VoronoiZoneRange> struct zone_info_range_traits {

	typedef typename boost::range_value<ZoneRange>::type zone_type;
	typedef typename boost::range_value<VoronoiZoneRange>::type voronoi_zone_type;
	typedef zone_info<zone_type, voronoi_zone_type> zone_info_type;
	typedef zone_info_builder<zone_info_type> zone_info_builder_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneRange, class VoronoiZoneRange> auto make_zone_info_range(ZoneRange &zones, VoronoiZoneRange &voronoi_zones) {

	typedef typename zone_info_range_traits<ZoneRange, VoronoiZoneRange>::zone_info_builder_type zone_info_builder_type;

	return util::make_zipped_with(zone_info_builder_type(), zones, voronoi_zones);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone, class VoronoiZone> class zone_info_policy {

	typedef std::vector<Zone> zone_container_type;

public:
	typedef zone_info<Zone, VoronoiZone> info_type;
	typedef typename zone_container_type::iterator iterator;
	typedef typename zone_container_type::const_iterator const_iterator;

	template <class VoronoiZoneRange> auto make_info_range(VoronoiZoneRange &voronoi_zones) {
		util::assign(zones, make_zone_range<Zone>(voronoi_zones));
		return make_zone_info_range(zones, voronoi_zones);
	}

	iterator begin() { return boost::begin(zones); }
	iterator end() { return boost::end(zones); }

	const_iterator begin() const { return boost::begin(zones); }
	const_iterator end() const { return boost::end(zones); }

	size_t size() const { return zones.size(); }

private:
	zone_container_type zones;
};

} // namespace geofis

#endif /* ZONE_INFO_HPP_ */
