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
#ifndef VORONOI_INFO_HPP_
#define VORONOI_INFO_HPP_

#include <util/address.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace geofis {

template <class VoronoiZone> struct voronoi_info {

	voronoi_info() {}
	voronoi_info(VoronoiZone &voronoi_zone) : voronoi_zone(voronoi_zone) {}

	template <class Geometry> void set_geometry(const Geometry &geometry) {
		voronoi_zone->set_geometry(geometry);
	}

	const VoronoiZone &get_voronoi_zone() const {
		return *voronoi_zone;
	}

	util::address_wrapper<VoronoiZone> voronoi_zone;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VoronoiZone> struct voronoi_info_builder {

	typedef voronoi_info<VoronoiZone> result_type;

	result_type operator()(VoronoiZone &voronoi_zone) const {
		return result_type(voronoi_zone);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VoronoiZone, class VoronoiZoneRange> struct voronoi_info_range_traits {

	typedef voronoi_info_builder<VoronoiZone> voronoi_info_builder_type;
	typedef boost::transformed_range<voronoi_info_builder_type, VoronoiZoneRange> voronoi_info_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VoronoiZone, class VoronoiZoneRange> typename voronoi_info_range_traits<VoronoiZone, VoronoiZoneRange>::voronoi_info_range_type make_voronoi_info_range(VoronoiZoneRange &voronoi_zones) {

	typedef typename voronoi_info_range_traits<VoronoiZone, VoronoiZoneRange>::voronoi_info_builder_type voronoi_info_builder_type;

	return boost::adaptors::transform(voronoi_zones, voronoi_info_builder_type());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VoronoiZone> struct voronoi_info_policy {

	typedef voronoi_info<VoronoiZone> info_type;

	template <class VoronoiZoneRange> typename voronoi_info_range_traits<VoronoiZone, VoronoiZoneRange>::voronoi_info_range_type make_info_range(VoronoiZoneRange &voronoi_zones) const {
		return make_voronoi_info_range<VoronoiZone>(voronoi_zones);
	}
};

} // namespace geofis

#endif /* VORONOI_INFO_HPP_ */
