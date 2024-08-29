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
#ifndef FUSION_MAP_HPP_
#define FUSION_MAP_HPP_

#include <vector>
#include <boost/mpl/if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <util/iterator/ref_iterator.hpp>
#include <geofis/algorithm/zoning/fusion/zone_fusion.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone_attributes_computer.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone_geometry_computer.hpp>
#include <util/range/unwrap_range.hpp>

#include <geofis/geometry/geometrical.hpp>

namespace geofis {

template <class Zone> class fusion_map;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone> struct fusion_map_traits {

	typedef boost::reference_wrapper<Zone> zone_reference_type;
	typedef std::vector<zone_reference_type> zone_reference_container_type;
	typedef zone_fusion<Zone> zone_fusion_type;
	typedef boost::reference_wrapper<zone_fusion_type> zone_fusion_reference_type;
	typedef fusion_map<Zone> fusion_map_type;
	typedef typename util::unwrap_range_traits<const zone_reference_container_type>::unwrap_range_type const_zone_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone> class fusion_map {

	typedef typename fusion_map_traits<Zone>::zone_reference_container_type zone_reference_container_type;
	typedef typename fusion_map_traits<Zone>::zone_fusion_type zone_fusion_type;
	typedef typename fusion_map_traits<Zone>::zone_fusion_reference_type zone_fusion_reference_type;
	typedef typename fusion_map_traits<Zone>::const_zone_range_type const_zone_range_type;

public:
	template <class ZoneRange> fusion_map(ZoneRange &zones, zone_fusion_type &zone_fusion, bool compute_zones = false) : zones(boost::begin(zones), boost::end(zones)), zone_fusion(zone_fusion) {
		if(compute_zones)
			this->compute_zones();
	}

	void compute_zones() {
		boost::for_each(zones, compute_zone_geometry(boost::unwrap_ref(zone_fusion)));
		boost::for_each(zones, compute_zone_attributes());
	}

	const_zone_range_type get_zones() const {
		return util::make_unwrap_range(zones);
	}

	size_t size() const {
		return zones.size();
	}

private:
	zone_reference_container_type zones;
	zone_fusion_reference_type zone_fusion;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneRange> struct fusion_map_maker_traits {

	typedef typename boost::range_value<ZoneRange>::type zone_value_type;
	typedef typename boost::mpl::if_<boost::is_reference_wrapper<zone_value_type>, typename boost::unwrap_reference<zone_value_type>::type, zone_value_type>::type zone_type;
	typedef typename fusion_map_traits<zone_type>::fusion_map_type fusion_map_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneRange, class FusionRange> inline typename fusion_map_maker_traits<ZoneRange>::fusion_map_type make_fusion_map(ZoneRange &zones, FusionRange &fusions) {

	typedef typename fusion_map_maker_traits<ZoneRange>::fusion_map_type fusion_map_type;

	return fusion_map_type(zones, fusions.back(), false);
}

} // namespace geofis

#endif /* FUSION_MAP_HPP_ */
