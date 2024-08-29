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
#ifndef H91AB50FF_B153_47AA_BD12_2735673DF243
#define H91AB50FF_B153_47AA_BD12_2735673DF243

#include <geofis/process/zoning/neighborhood/neighborhood_process_traits.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighbor_maker.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighboring.hpp>
#include <geofis/algorithm/zoning/triangulation/edge_to_zone_adaptor.hpp>

namespace geofis {

class neighborhood_process_impl {

	typedef neighborhood_process_traits::zone_type zone_type;
	typedef neighborhood_process_traits::neighborhood_type neighborhood_type;
	typedef neighborhood_process_traits::zone_neighbor_container_type zone_neighbor_container_type;
	typedef neighborhood_process_traits::finite_edge_range_type finite_edge_range_type;
	typedef neighborhood_process_traits::zone_neighbor_range_type zone_neighbor_range_type;
	typedef neighborhood_process_traits::const_zone_neighbor_range_type const_zone_neighbor_range_type;
	typedef zone_neighbor_maker<zone_type> zone_neighbor_maker_type;
	typedef edge_to_zone_adaptor<zone_neighbor_maker_type> edge_to_zone_neighbor_maker_type;
	typedef edge_to_zone_adaptor<neighborhood_type> edge_to_zone_neighborhood_type;
	typedef zone_neighboring zone_neighboring_type;
	typedef edge_to_zone_adaptor<zone_neighboring_type> edge_to_zone_neighboring_type;

	zone_neighbor_container_type zone_neighbors;
	zone_neighbor_container_type filtered_zone_neighbors;

public:
	neighborhood_process_impl();
	neighborhood_process_impl(const neighborhood_type &neighborhood, const finite_edge_range_type &finite_edges);
	~neighborhood_process_impl();

	zone_neighbor_range_type get_zone_neighbors();
	const_zone_neighbor_range_type get_zone_neighbors() const;
	const_zone_neighbor_range_type get_filtered_zone_neighbors() const;
};

} // namespace geofis

#endif // H91AB50FF_B153_47AA_BD12_2735673DF243 
