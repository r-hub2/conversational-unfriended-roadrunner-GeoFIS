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
#ifndef H923AD8BC_C173_4A0D_9993_458477B7771B
#define H923AD8BC_C173_4A0D_9993_458477B7771B

#include <string>
#include <vector>
#include <list>
#include <boost/range.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <geofis/data/feature.hpp>
#include <geofis/algorithm/zoning/geometry/zoning_geometry_traits.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone.hpp>
#include <geofis/algorithm/zoning/fusion/voronoi/zone_info.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_zone.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_map.hpp>
#include <geofis/algorithm/zoning/neighborhood/variant_neighborhood.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighbor.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/variant_aggregation.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_zone_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_multidimensional_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_attribute_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_feature_distance.hpp>
#include <geofis/algorithm/zoning/fusion/zone_fusion.hpp>
#include <geofis/algorithm/zoning/fusion/map/fusion_map_range.hpp>
#include <geofis/algorithm/zoning/merge/variant_merge.hpp>
#include <geofis/algorithm/zoning/map/map.hpp>

namespace geofis {

struct zoning_process_traits {

	typedef zoning_geometry_traits::kernel_type kernel_type;
	typedef zoning_geometry_traits::point_type point_type;
	typedef zoning_geometry_traits::polygon_type polygon_type;
	typedef zoning_geometry_traits::polygon_with_holes_type polygon_with_holes_type;

	typedef feature<std::string, point_type, std::vector<double> > feature_type;
	typedef std::vector<feature_type> feature_container_type;
	typedef boost::sub_range<feature_container_type> feature_range_type;

	typedef voronoi_zone<polygon_type, feature_type> voronoi_zone_type;

	typedef zone<polygon_with_holes_type, voronoi_zone_type> zone_type;

	typedef zone_info_policy<zone_type, voronoi_zone_type> zone_info_policy_type;

	typedef voronoi_map<kernel_type, feature_type, zone_info_policy_type> voronoi_map_type;
	typedef voronoi_map_type::finite_edge_range_type finite_edge_range_type;

	typedef variant_neighborhood neighborhood_type;

	typedef zone_neighbor<zone_type> zone_neighbor_type;
	typedef std::vector<zone_neighbor_type> zone_neighbor_container_type;
	typedef boost::sub_range<zone_neighbor_container_type> zone_neighbor_range_type;
	typedef boost::sub_range<const zone_neighbor_container_type> const_zone_neighbor_range_type;

	typedef variant_aggregation aggregation_type;

	typedef variant_zone_distance zone_distance_type;

	typedef variant_multidimensional_distance multidimensional_distance_type;

	typedef variant_attribute_distance attribute_distance_type;
	typedef std::vector<attribute_distance_type> attribute_distance_container_type;
	typedef boost::sub_range<attribute_distance_container_type> attribute_distance_range_type;
	typedef boost::sub_range<const attribute_distance_container_type> const_attribute_distance_range_type;

	typedef typename variant_feature_distance_traits<multidimensional_distance_type, attribute_distance_type>::variant_feature_distance_type feature_distance_type;

	typedef zone_fusion<zone_type> zone_fusion_type;
	typedef typename fusion_map_traits<zone_type>::fusion_map_type fusion_map_type;
	typedef std::list<zone_fusion_type> zone_fusion_container_type;
	typedef typename fusion_map_range_traits<zone_fusion_container_type>::fusion_map_range_type fusion_map_range_type;
	typedef typename boost::reversed_range<const fusion_map_range_type> reverse_fusion_map_range_type;

	typedef variant_merge merge_type;
	typedef map<zone_type> merge_map_type;
};

} // namespace geofis

#endif // H923AD8BC_C173_4A0D_9993_458477B7771B 
