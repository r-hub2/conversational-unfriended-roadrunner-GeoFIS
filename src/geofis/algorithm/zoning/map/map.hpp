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
#ifndef HC58DECC0_88E7_4EC2_8D78_9D5BA519468D
#define HC58DECC0_88E7_4EC2_8D78_9D5BA519468D

#include <geofis/algorithm/zoning/map/map_traits.hpp>

namespace geofis {

template <class Zone> class map {

	typedef typename map_traits<Zone>::zone_container_type zone_container_type;
	typedef typename map_traits<Zone>::const_zone_range_type const_zone_range_type;

public:
	map() {}
	template <class ZoneRange> map(const ZoneRange &zones) : zones(boost::begin(zones), boost::end(zones)) {}

	size_t size() const {
		return zones.size();
	}

	const_zone_range_type get_zones() const {
		return zones;
	}

private:
	zone_container_type zones;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of map with reference_wrapper of zone

template <class Zone> class map<boost::reference_wrapper<Zone> > {

	typedef typename map_traits<boost::reference_wrapper<Zone> >::zone_container_type zone_container_type;
	typedef typename map_traits<boost::reference_wrapper<Zone> >::const_zone_range_type const_zone_range_type;

public:
	map() {}
	template <class ZoneRange> map(const ZoneRange &zones) : zones(boost::begin(zones), boost::end(zones)) {}

	size_t size() const {
		return zones.size();
	}

	const_zone_range_type get_zones() const {
		return util::make_unwrap_range(zones);
	}

private:
	zone_container_type zones;
};

} // namespace geofis

#endif // HC58DECC0_88E7_4EC2_8D78_9D5BA519468D 
