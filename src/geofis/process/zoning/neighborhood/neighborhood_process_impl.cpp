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
#include <geofis/process/zoning/neighborhood/neighborhood_process_impl.hpp>
#include <boost/algorithm/cxx11/partition_copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION <= 107300
#include <boost/function_output_iterator.hpp>
#else
#include <boost/iterator/function_output_iterator.hpp>
#endif

using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace util;

namespace geofis {

template <class EdgeToZoneNeighbor, class Container> struct edge_to_zone_neighbor_inserter {

	const EdgeToZoneNeighbor &edge_to_zone_neighbor;
	Container &container;

	edge_to_zone_neighbor_inserter(const EdgeToZoneNeighbor &edge_to_zone_neighbor, Container &container) : edge_to_zone_neighbor(edge_to_zone_neighbor), container(container) {}

	template <class Edge> void operator() (const Edge &edge) const {
		container.push_back(edge_to_zone_neighbor(edge));
	}
};

template <class EdgeToZoneNeighbor, class Container> inline auto make_edge_to_zone_neighbor_inserter(const EdgeToZoneNeighbor &edge_to_zone_neighbor, Container &container) {
	return make_function_output_iterator(edge_to_zone_neighbor_inserter<EdgeToZoneNeighbor, Container>(edge_to_zone_neighbor, container));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

neighborhood_process_impl::neighborhood_process_impl() {}

neighborhood_process_impl::neighborhood_process_impl(const neighborhood_type &neighborhood, const finite_edge_range_type &finite_edges) {
	edge_to_zone_neighborhood_type edge_to_zone_neighborhood(neighborhood);
	zone_neighbor_maker_type zone_neighbor_maker;
	edge_to_zone_neighbor_maker_type edge_to_zone_neighbor_maker(zone_neighbor_maker);
	zone_neighboring_type zone_neighboring;
	edge_to_zone_neighboring_type edge_to_zone_neighboring(zone_neighboring);
	partition_copy(finite_edges | filtered(edge_to_zone_neighboring), make_edge_to_zone_neighbor_inserter(edge_to_zone_neighbor_maker, zone_neighbors), make_edge_to_zone_neighbor_inserter(edge_to_zone_neighbor_maker, filtered_zone_neighbors), edge_to_zone_neighborhood);
}

neighborhood_process_impl::~neighborhood_process_impl() {}

neighborhood_process_impl::zone_neighbor_range_type neighborhood_process_impl::get_zone_neighbors() {
	return zone_neighbors;
}

neighborhood_process_impl::const_zone_neighbor_range_type neighborhood_process_impl::get_zone_neighbors() const {
	return zone_neighbors;
}

neighborhood_process_impl::const_zone_neighbor_range_type neighborhood_process_impl::get_filtered_zone_neighbors() const {
	return filtered_zone_neighbors;
}

} // namespace geofis
