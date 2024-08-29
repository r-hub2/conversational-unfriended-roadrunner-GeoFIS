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
#include <geofis/process/zoning/neighborhood/neighborhood_process.hpp>
#include <geofis/process/zoning/neighborhood/neighborhood_process_impl.hpp>

using namespace boost;

namespace geofis {

neighborhood_process::neighborhood_process() : impl(nullptr) {}

neighborhood_process::neighborhood_process(const neighborhood_type &neighborhood, const finite_edge_range_type &finite_edges) : impl(new neighborhood_process_impl(neighborhood, finite_edges)) {}

neighborhood_process::~neighborhood_process() {}

neighborhood_process &neighborhood_process::move_assign(neighborhood_process &other) {
	impl = boost::move(other.impl);
	return *this;
}

neighborhood_process::zone_neighbor_range_type neighborhood_process::get_zone_neighbors() {
	return impl->get_zone_neighbors();
}

neighborhood_process::const_zone_neighbor_range_type neighborhood_process::get_zone_neighbors() const {
	return impl->get_zone_neighbors();
}

neighborhood_process::const_zone_neighbor_range_type neighborhood_process::get_filtered_zone_neighbors() const {
	return impl->get_filtered_zone_neighbors();
}

} // namespace geofis
