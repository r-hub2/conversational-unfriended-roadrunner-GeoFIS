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
#ifndef H376F278A_DCAD_4A78_A550_CD7E2F1D2F24
#define H376F278A_DCAD_4A78_A550_CD7E2F1D2F24

#include <geofis/algorithm/zoning/fusion/zone_fusion.hpp>
#include <CGAL/Boolean_set_operations_2.h>
#include <util/address_equal.hpp>
#include <util/assert.hpp>

namespace geofis {

template <class ZoneFusion> class zone_geometry_computer {

	typedef typename ZoneFusion::zone_type zone_type;
	typedef typename zone_type::geometry_type geometry_type;

public:
	zone_geometry_computer(ZoneFusion &zone_fusion) : zone_fusion(zone_fusion) {}

	void operator()(zone_type &zone) {
		if(is_zone_in_zone_fusion(zone))
			compute_zone_fusion_geometry();
		 else
			 zone.compute_geometry();
	}

private:
	ZoneFusion &zone_fusion;

	bool is_zone_in_zone_fusion(const zone_type &zone) {
		return zone_fusion.contain_zone(zone) || util::address_equal(zone, zone_fusion.get_fusion());
	}

	void compute_zone_fusion_geometry() {
		compute_zone_fusion_geometry(zone_fusion.get_zone1(), zone_fusion.get_zone2(), zone_fusion.get_fusion());
	}

	void compute_zone_fusion_geometry(zone_type &zone1, zone_type &zone2, zone_type &fusion) {
		if(!fusion.has_geometry()) {
			zone1.compute_geometry();
			zone2.compute_geometry();
			join_zones(fusion, zone1, zone2);
		} else {
			if(zone1.has_geometry() && zone2.has_geometry())
				return;
			if(zone1.has_geometry())
				difference_zones(zone2, fusion, zone1);
			else if(zone2.has_geometry())
				difference_zones(zone1, fusion, zone2);
			else {
				if(zone1.size() < zone2.size()) {
					zone1.compute_geometry();
					difference_zones(zone2, fusion, zone1);
				} else {
					zone2.compute_geometry();
					difference_zones(zone1, fusion, zone2);
				}
			}
		}
	}

	void join_zones(zone_type &zone_result, const zone_type &zone1, const zone_type &zone2) {
		geometry_type join_result;
		UTIL_ALLEGE(CGAL::join(zone_fusion.get_zone1().get_geometry(), zone_fusion.get_zone2().get_geometry(), join_result));
		zone_result.set_geometry(join_result);
	}

	void difference_zones(zone_type &zone_result, const zone_type &zone1, const zone_type &zone2) {
		std::vector<geometry_type> difference_results;
		CGAL::difference(zone1.get_geometry(), zone2.get_geometry(), std::back_inserter(difference_results));
		UTIL_ASSERT(difference_results.size() == 1);
		zone_result.set_geometry(difference_results[0]);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneFusion> inline zone_geometry_computer<ZoneFusion> compute_zone_geometry(ZoneFusion &zone_fusion) {
	return zone_geometry_computer<ZoneFusion>(zone_fusion);
}

} // namespace geofis

#endif // H376F278A_DCAD_4A78_A550_CD7E2F1D2F24 
