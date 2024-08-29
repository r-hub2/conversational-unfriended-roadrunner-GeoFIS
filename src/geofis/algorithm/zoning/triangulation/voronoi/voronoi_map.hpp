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
#ifndef VORONOI_MAP_HPP_
#define VORONOI_MAP_HPP_

#include <vector>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility.hpp>
#include <boost/range/sub_range.hpp>
#include <util/assert.hpp>
#include <util/range/assign.hpp>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_zone_traits.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_traits.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_info.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/face_to_geometry.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/face_to_polygon.hpp>

namespace geofis {

template <class Kernel, class Feature> struct voronoi_map_traits {

	typedef typename voronoi_zone_traits<Kernel, Feature>::voronoi_zone_type voronoi_zone_type;
	typedef voronoi_info_policy<voronoi_zone_type> voronoi_info_policy_type;
	typedef typename boost::add_const<voronoi_info_policy_type>::type default_info_policy_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel, class Feature, class InfoPolicy = typename voronoi_map_traits<Kernel, Feature>::default_info_policy_type> class voronoi_map : public boost::noncopyable {

	typedef typename voronoi_zone_traits<Kernel, Feature>::geometry_type geometry_type;
	typedef typename voronoi_zone_traits<Kernel, Feature>::voronoi_zone_type voronoi_zone_type;
	typedef typename InfoPolicy::info_type info_type;
	typedef typename voronoi_traits<Kernel, info_type>::delaunay_triangulation_type delaunay_triangulation_type;
	typedef typename voronoi_traits<Kernel, info_type>::voronoi_diagram_type voronoi_diagram_type;
	typedef typename delaunay_triangulation_type::Finite_edges_iterator finite_edge_iterator;
	typedef std::vector<voronoi_zone_type> zone_container_type;

public:
	typedef typename boost::sub_range<const zone_container_type>::type const_zone_range_type;
	typedef typename boost::iterator_range<finite_edge_iterator> finite_edge_range_type;

	template <class FeatureRange> void initialize(const FeatureRange &features, const geometry_type &boundary, InfoPolicy &info_policy) {
		initialize(make_voronoi_zone_range<geometry_type>(features), features, boundary, info_policy);
	}

	template <class FeatureRange> void initialize(const FeatureRange &features, const geometry_type &boundary) {
		initialize(features, boundary, InfoPolicy());
	}

	size_t size() const { return zones.size(); }

	const_zone_range_type get_zones() const { return zones; }

	finite_edge_range_type get_finite_edges() const {
		return boost::make_iterator_range(delaunay.finite_edges_begin(), delaunay.finite_edges_end());
	}

private:
	zone_container_type zones;
	delaunay_triangulation_type delaunay;

	template <class ZoneRange, class FeatureRange> void initialize(const ZoneRange &zones, const FeatureRange &features, const geometry_type &boundary, InfoPolicy &info_policy) {
		util::assign(this->zones, zones);
		initialize_delaunay(features, info_policy);
		initialize_zone_geometries_with_voronoi(boundary);
	}

	template <class FeatureRange> void initialize_delaunay(const FeatureRange &features, InfoPolicy &info_policy) {
		initialize_delaunay_with_infos(make_geometry_range(features), info_policy.make_info_range(zones));
	}

	template <class GeometryRange, class InfoRange> void initialize_delaunay_with_infos(const GeometryRange &geometries, const InfoRange &infos) {
		delaunay.clear();
		delaunay.insert(boost::make_zip_iterator(boost::make_tuple(boost::begin(geometries), boost::begin(infos))), boost::make_zip_iterator(boost::make_tuple(boost::end(geometries), boost::end(infos))));
	}

	void initialize_zone_geometries_with_voronoi(const geometry_type &boundary) {
		initialize_zone_geometries_with_voronoi(voronoi_diagram_type(delaunay), boundary);
	}

	void initialize_zone_geometries_with_voronoi(const voronoi_diagram_type &voronoi, const geometry_type &boundary) {
		std::for_each(voronoi.faces_begin(), voronoi.faces_end(), make_face_to_geometry(make_face_to_polygon(boundary)));
	}
};

} // namespace geofis

#endif /* VORONOI_MAP_HPP_ */
