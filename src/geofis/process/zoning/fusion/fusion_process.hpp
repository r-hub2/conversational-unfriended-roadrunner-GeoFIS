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
#ifndef H6D16940F_AC56_4C71_A15E_09AEAA8C7382
#define H6D16940F_AC56_4C71_A15E_09AEAA8C7382

#include <boost/move/unique_ptr.hpp>
#include <geofis/process/zoning/fusion/fusion_process_traits.hpp>

namespace geofis {

class fusion_process_impl;

class fusion_process {

	typedef fusion_process_traits::aggregation_type aggregation_type;
	typedef fusion_process_traits::zone_distance_type zone_distance_type;
	typedef fusion_process_traits::multidimensional_distance_type multidimensional_distance_type;
	typedef fusion_process_traits::attribute_distance_range_type attribute_distance_range_type;
	typedef fusion_process_traits::feature_range_type feature_range_type;
	typedef fusion_process_traits::zone_neighbor_range_type zone_neighbor_range_type;
	typedef fusion_process_traits::zone_info_policy_type zone_info_policy_type;
	typedef fusion_process_traits::fusion_map_range_type fusion_map_range_type;

	BOOST_MOVABLE_BUT_NOT_COPYABLE(fusion_process)

	boost::movelib::unique_ptr<fusion_process_impl> impl;

public:
	fusion_process();
	fusion_process(const aggregation_type &aggregation, const zone_distance_type &zone_distance, const multidimensional_distance_type &multidimensional_distance, attribute_distance_range_type attribute_distances, feature_range_type &features, zone_neighbor_range_type zone_neighbors);
	~fusion_process();

	fusion_process & operator= (BOOST_RV_REF(fusion_process) other) {
		// see bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53725
		return move_assign(other);
	}

	size_t get_fusion_size() const;
	fusion_map_range_type get_fusion_maps(zone_info_policy_type &zones, size_t begin, size_t end, bool compute_zones = false);

	bool is_implemented() const {
		return impl;
	}

private:
	fusion_process &move_assign(fusion_process &other);
};

} // namespace geofis

#endif // H6D16940F_AC56_4C71_A15E_09AEAA8C7382 
