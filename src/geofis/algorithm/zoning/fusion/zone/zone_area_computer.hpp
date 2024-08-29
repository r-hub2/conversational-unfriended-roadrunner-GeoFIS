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
#ifndef H0154A9AD_1609_4CFA_B20D_3169F446CD87
#define H0154A9AD_1609_4CFA_B20D_3169F446CD87

namespace geofis {

template <class Zone> class zone_area_computer {

	Zone &zone;
	const Zone &zone1;
	const Zone &zone2;

public:
	zone_area_computer(Zone &zone, const Zone &zone1, const Zone &zone2) : zone(zone), zone1(zone1), zone2(zone2) {}

	void compute_area() const {
		zone.set_area(zone1.get_area() + zone2.get_area());
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Zone> inline void compute_zone_area(Zone &zone, const Zone &zone1, const Zone &zone2) {
	zone_area_computer<Zone>(zone, zone1, zone2).compute_area();
}

} // namespace geofis

#endif // H0154A9AD_1609_4CFA_B20D_3169F446CD87 
