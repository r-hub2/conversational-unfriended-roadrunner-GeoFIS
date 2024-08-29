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
#ifndef H72C4931E_D166_4DAA_BADE_38F16FA32452
#define H72C4931E_D166_4DAA_BADE_38F16FA32452

#include <boost/move/unique_ptr.hpp>
#include <geofis/process/zoning/voronoi/voronoi_process_traits.hpp>

namespace geofis {

class voronoi_process_impl;

class voronoi_process {

	BOOST_MOVABLE_BUT_NOT_COPYABLE(voronoi_process)

	typedef voronoi_process_traits::feature_range_type feature_range_type;
	typedef voronoi_process_traits::polygon_type polygon_type;
	typedef voronoi_process_traits::zone_info_policy_type zone_info_policy_type;
	typedef voronoi_process_traits::voronoi_map_type voronoi_map_type;
	typedef voronoi_process_traits::finite_edge_range_type finite_edge_range_type;

	boost::movelib::unique_ptr<voronoi_process_impl> impl;

public:
	voronoi_process();
	voronoi_process(const feature_range_type &features, const polygon_type &border);
	~voronoi_process();

	voronoi_process & operator= (BOOST_RV_REF(voronoi_process) other) {
		// see bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53725
		return move_assign(other);
	}

	zone_info_policy_type &get_zones();

	const voronoi_map_type &get_voronoi_map() const;
	finite_edge_range_type get_finite_edges() const;

	bool is_implemented() const {
		return impl;
	}

private:
	voronoi_process &move_assign(voronoi_process &other);
};

} // namespace geofis

#endif // H72C4931E_D166_4DAA_BADE_38F16FA32452 
