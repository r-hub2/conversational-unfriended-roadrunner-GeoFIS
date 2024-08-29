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
#ifndef EDGE_TO_INFO_ADAPTOR_HPP_
#define EDGE_TO_INFO_ADAPTOR_HPP_

#include <util/functional/binary_adaptor.hpp>
#include <CGAL/Triangulation_utils_2.h>

namespace geofis {

template <class Binary> class edge_to_info_adaptor {

public:
	typedef typename util::binary_adaptor_traits<Binary>::result_type result_type;

	edge_to_info_adaptor() {}
	edge_to_info_adaptor(const Binary &binary) : binary(binary) {}

	template <class Edge> result_type operator() (Edge &edge) const {
		return result_vertex(edge.first->vertex(CGAL::Triangulation_cw_ccw_2::ccw(edge.second)), edge.first->vertex(CGAL::Triangulation_cw_ccw_2::cw(edge.second)));
	}

private:
	util::binary_adaptor<Binary> binary;

	template <class Info> result_type result_infos(Info &info1, Info &info2) const {
		return binary(info1, info2);
	}

	template <class Vertex> result_type result_vertex(const Vertex &vertex1, const Vertex &vertex2) const {
		return result_infos(vertex1->info(), vertex2->info());
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary> struct edge_to_info_adaptor<Binary> make_edge_to_info_adaptor(const Binary &binary) {
	return edge_to_info_adaptor<Binary>(binary);
}

} // namespace geofis

#endif /* EDGE_TO_INFO_ADAPTOR_HPP_ */
