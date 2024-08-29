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
#ifndef H240DF543_434E_4E82_84FB_4330ED8B8AC5
#define H240DF543_434E_4E82_84FB_4330ED8B8AC5

#include <Rcpp.h>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/rcpp/geometry/polygons.hpp>

namespace geofis {

template <class VoronoiMap> Rcpp::S4 make_rcpp_voronoi_map(const VoronoiMap &voronoi_map, const Rcpp::S4 &crs) {
	auto geometry_range = make_geometry_range(voronoi_map.get_zones());
	Rcpp::List Srl = make_rcpp_polygons_list(geometry_range);
	Rcpp::Function rebuild_crs("rebuild_CRS");
	Rcpp::S4 proj4string = rebuild_crs(crs);
	Rcpp::Function spatial_polygons("SpatialPolygons");
	return spatial_polygons(Rcpp::Named("Srl") = Srl, Rcpp::Named("proj4string") = proj4string);
}

} // namespace geofis

#endif // H240DF543_434E_4E82_84FB_4330ED8B8AC5 
