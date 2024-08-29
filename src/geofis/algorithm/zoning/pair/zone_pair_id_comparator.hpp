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
#ifndef H4D3F2274_06FE_4E2B_B7C0_EBCB80EE3005
#define H4D3F2274_06FE_4E2B_B7C0_EBCB80EE3005

#include <algorithm>
#include <geofis/identifiable/id_equal.hpp>
#include <geofis/identifiable/identifiable_comparator.hpp>

namespace geofis {

struct zone_pair_id_comparator {

	template <class ZonePair> bool operator() (const ZonePair &lhs, const ZonePair &rhs) const {

		typedef typename ZonePair::zone_type zone_type;
		typedef typename zone_type::id_type id_type;

		id_type min_lhs = std::min(lhs.get_zone1(), lhs.get_zone2(), identifiable_comparator()).get_id();
		id_type max_lhs = std::max(lhs.get_zone1(), lhs.get_zone2(), identifiable_comparator()).get_id();
		id_type min_rhs = std::min(rhs.get_zone1(), rhs.get_zone2(), identifiable_comparator()).get_id();
		id_type max_rhs = std::max(rhs.get_zone1(), rhs.get_zone2(), identifiable_comparator()).get_id();

		return id_equal()(min_lhs, min_rhs) ? id_comparator()(max_lhs, max_rhs) : id_comparator()(min_lhs, min_rhs);
	}
};

} // namespace geofis

#endif // H4D3F2274_06FE_4E2B_B7C0_EBCB80EE3005 
