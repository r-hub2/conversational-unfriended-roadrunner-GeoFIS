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
#ifndef ZONING_HPP_
#define ZONING_HPP_

#include <boost/range.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/assign/list_of.hpp>
#include <util/functional/unary_adaptor.hpp>
#include <util/functional/binary_adaptor.hpp>
#include <util/range/assign.hpp>
#include <util/range/ref_range.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION <= 107300
#include <boost/function_output_iterator.hpp>
#else
#include <boost/iterator/function_output_iterator.hpp>
#endif
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/stable_sort.hpp>
#include <boost/algorithm/cxx11/partition_copy.hpp>
#include <geofis/geometry/polygon_with_holes.hpp>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/geometry/geometrical_equal.hpp>
#include <geofis/geometry/geometrical_comparator.hpp>
#include <geofis/data/geometry_iterator.hpp>
#include <geofis/algorithm/feature/feature_normalization.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighboring.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_multidimensional_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_attribute_distance.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone.hpp>
#include <geofis/algorithm/zoning/fusion/zone/zone_traits.hpp>
#include <geofis/algorithm/zoning/fusion/voronoi/zone_info.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/feature_distance.hpp>
#include <geofis/algorithm/zoning/fusion/fusioning.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_zone_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_multidimensional_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_feature_distance.hpp>

//#include <geofis/algorithm/zoning/variant_zone_pair_distance.hpp>
//#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_traits.hpp>
//#include <geofis/algorithm/zoning/triangulation/voronoi/face_to_geometry.hpp>
//#include <geofis/algorithm/zoning/triangulation/voronoi/face_to_polygon.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/voronoi_map.hpp>
//#include <geofis/algorithm/zoning/triangulation/edge_to_zone_pair.hpp>
#include <geofis/algorithm/zoning/triangulation/edge_to_zone_adaptor.hpp>
//#include <geofis/algorithm/zoning/fusion/aggregation/zoning_aggregation.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/aggregation_adaptor.hpp>
#include <geofis/algorithm/zoning/fusion/aggregation/variant_aggregation.hpp>
//#include <geofis/algorithm/zoning/fusion/fusion.hpp>
#include <geofis/algorithm/zoning/fusion/zone_fusion.hpp>
#include <geofis/algorithm/zoning/fusion/neighbor_to_zone_pair.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance_updater.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_updater.hpp>
//#include <geofis/algorithm/zoning/variant_zone_filter.hpp>
#include <geofis/algorithm/zoning/neighborhood/variant_neighborhood.hpp>
#include <geofis/algorithm/zoning/neighborhood/zone_neighbor_maker.hpp>
#include <geofis/identifiable/identifiable_comparator.hpp>
#include <geofis/algorithm/zoning/fusion/map/fusion_map_range.hpp>
#include <geofis/algorithm/zoning/fusion/map/fusion_map.hpp>
#include <CGAL/spatial_sort.h>
/*#include <geofis/algorithm/normalize.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <geofis/data/geometry_iterator.hpp>*/


//#include <boost/ptr_container/ptr_vector.hpp>
//#include <geofis/algorithm/distance/virtual_attribute_distance.hpp>
//#include <geofis/algorithm/distance/virtual_feature_distance.hpp>

#include <util/type.hpp>

#include <list>
#include <iostream>
#include <util/type.hpp>
//#include <adobe/timer.hpp>

#include <boost/range/algorithm/unique.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <CGAL/Boolean_set_operations_2/Gps_polygon_validation.h>
#include <CGAL/Timer.h>


//#define toto

using namespace boost::posix_time;

namespace geofis {

template <class Kernel, class Feature> struct zoning_traits {

	typedef typename voronoi_zone_traits<Kernel, Feature>::voronoi_zone_type voronoi_zone_type;
	typedef typename zone_traits<Kernel, voronoi_zone_type>::zone_type zone_type;
	typedef zone_info_policy<zone_type, voronoi_zone_type> zone_info_policy_type;
	typedef voronoi_map<Kernel, Feature, zone_info_policy_type> voronoi_map_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Feature, class Zone> struct fusioning_traits {

	//typedef minimum_aggregation aggregation_type;
	typedef variant_aggregation aggregation_type;
	//typedef util::minimum<double> zone_distance_type;
	typedef variant_zone_distance zone_distance_type; // TODO probl�me de timing +100ms sur rdt4 en variant_zone_distance
	//typedef util::euclidean_distance<double> multidimensional_distance_type;
	typedef variant_multidimensional_distance multidimensional_distance_type;
	//typedef util::euclidean_distance<double> attribute_distance_type;
	typedef variant_attribute_distance attribute_distance_type;
	//typedef feature_distance<void, attribute_distance_type> feature_distance_type;
	//typedef feature_distance<multidimensional_distance_type, attribute_distance_type> feature_distance_type;
	typedef typename variant_feature_distance_traits<multidimensional_distance_type, attribute_distance_type>::variant_feature_distance_type feature_distance_type;
	typedef feature_normalization<Feature> feature_normalization_type;
	typedef zone_pair_distance<zone_distance_type> zone_pair_distance_type;
	typedef zone_pair<Zone, zone_pair_distance_type> zone_pair_type;
	typedef neighbor_to_zone_pair<Zone, zone_distance_type, feature_distance_type> neighbor_to_zone_pair_type;
	typedef zone_fusion<Zone> zone_fusion_type;
	typedef zone_pair_distance_updater<feature_distance_type> zone_pair_distance_updater_type;
	typedef zone_pair_updater<zone_pair_distance_updater_type> zone_pair_updater_type;

	typedef fusioning<fusioning_traits> fusioning_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel, class Feature> class zoning {

	typedef std::vector<Feature> feature_container_type;
	typedef typename boost::sub_range<feature_container_type> feature_range_type;
	typedef CGAL::Polygon_2<Kernel> boundary_type;
	typedef typename zoning_traits<Kernel, Feature>::zone_info_policy_type voronoi_zones_type;
	typedef typename zoning_traits<Kernel, Feature>::voronoi_zone_type voronoi_zone_type;
	typedef typename zoning_traits<Kernel, Feature>::voronoi_map_type voronoi_map_type;
	typedef variant_neighborhood neighborhood_type;
	typedef typename zoning_traits<Kernel, Feature>::zone_type zone_type;
	typedef zone_neighbor<zone_type> zone_neighbor_type;
	typedef std::vector<zone_neighbor_type> zone_neighbor_container_type;

	typedef typename fusioning_traits<Feature, zone_type>::fusioning_type fusioning_type;
	typedef typename fusioning_traits<Feature, zone_type>::zone_fusion_type zone_fusion_type;

	typedef std::list<zone_fusion_type> zone_fusion_container_type;

	typedef typename fusion_map_range_traits<zone_fusion_container_type>::fusion_map_range_type fusion_map_range_type;
	typedef typename boost::reversed_range<const fusion_map_range_type> reverse_fusion_map_range_type;

public:
	typedef typename boost::sub_range<const zone_neighbor_container_type> zone_neighbor_range_type;

	//zoning() {}
	template <class FeatureRange> zoning(const FeatureRange &features) : features(boost::begin(features), boost::end(features)) {
		boost::stable_sort(this->features, geometrical_comparator<Feature>());
		unique_features = boost::unique<boost::return_begin_found>(this->features, geometrical_equal());
	}

#ifdef iuyo
	template <class FeatureRange> void set_features(const FeatureRange &features) {
		//std::cout << "set_features\n";
		/*UTIL_REQUIRE(is_valid_features(features));
		std::vector<Feature> new_features(boost::begin(features), boost::end(features));
		if(has_boundary()) {
			boost::stable_sort(new_features, geometrical_comparator<Feature>());
			feature_range_type unique_new_features = boost::unique<boost::return_begin_found>(new_features, geometrical_equal());
			boost::stable_partition<boost::return_begin_found>(unique_new_features, is_feature_bounded(boundary));
		}
		if(!has_features())
			assign_features(new_features);
		else
			update_features(new_features);*/
		// TODO reset neighboring et fusioning
		util::assign(this->features, features);
		//fusioning.release();
	}
#endif

	void set_boundary(const boundary_type &boundary) {
		//std::cout << "set_boundary\n";
		UTIL_REQUIRE(is_valid_boundary(boundary));
		this->boundary = boundary;
		bounded_features = boost::stable_partition<boost::return_begin_found>(unique_features, is_feature_bounded(this->boundary));
		/*if(has_features()) {
			boost::sort(features, identifiable_comparator());
			//adobe::timer_t timer;
			boost::stable_sort(features, geometrical_comparator<Feature>());
			//timer.report("geometrical_sorting");
			//std::cout << boost::distance(features) << " features\n";
			unique_features = boost::unique<boost::return_begin_found>(features, geometrical_equal());
			//std::cout << boost::distance(unique_features) << " unique features\n";
			bounded_features = boost::stable_partition<boost::return_begin_found>(unique_features, is_feature_bounded(boundary));
		}*/
		//	prepare_features();
		// TODO reset voronoi neighboring et fusioning
		//fusioning.release();
	}

	/*void prepare_features() {
		fusioning.release();
		UTIL_REQUIRE(is_valid_boundary(boundary));
		boost::sort(features, identifiable_comparator());
		boost::stable_sort(features, geometrical_comparator<Feature>());
		unique_features = boost::unique<boost::return_begin_found>(features, geometrical_equal());
		bounded_features = boost::stable_partition<boost::return_begin_found>(unique_features, is_feature_bounded(boundary));
	}*/

	void compute_voronoi() {
		voronoi_map.initialize(bounded_features, boundary, voronoi_zones);
	}

	const voronoi_map_type &get_voronoi_map() const {
		return voronoi_map;
	}

	template <class Neighborhood> void set_neighborhood(const Neighborhood &neighborhood) {
		this->neighborhood = neighborhood;
		// TODO reset neighboring et fusioning
		fusioning.release();
	}

	void clear_zone_neighbors() {
		zone_neighbors.clear();
		filtered_zone_neighbors.clear();
	}

	void compute_neighborhood() {
		//adobe::timer_t timer;
		clear_zone_neighbors();
		//initialize_zone_neighbors_with_finite_edges();
		initialize_zone_neighbors_with_finite_edges(zone_neighbors, filtered_zone_neighbors);
		//timer.report("neighborhood");
		//std::cout << "zone_neighbors.size " << zone_neighbors.size() << std::endl;
		//std::cout << "filtered_zone_neighbors.size " << filtered_zone_neighbors.size() << std::endl;
	}

	void compute_fusion() {
		fusioning.compute(bounded_features, zone_neighbors);
	}

	zone_neighbor_range_type get_zone_neighbors() const {
		return zone_neighbors;
	}

	zone_neighbor_range_type get_filtered_zone_neighbors() const {
		return filtered_zone_neighbors;
	}

	struct print_attribute_distance {

		struct attribute_distance__printer_visitor : public boost::static_visitor<> {

			void operator()(const util::euclidean_distance<double> &) const {
				std::cout << "euclidean_distance\n";
			}
			void operator()(const fispro::fuzzy_distance &) const {
				std::cout << "fuzzy_distance\n";
			}
		};

		template <class AttributeDistance> void operator()(const AttributeDistance &attribute_distance) {
			boost::apply_visitor(attribute_distance__printer_visitor(), attribute_distance);
		}
	};

	template <class Aggregation> void set_aggregation(const Aggregation &aggregation) {
		fusioning.set_aggregation(aggregation);
	}

	template <class AttributeDistanceRange> void set_attribute_distances(const AttributeDistanceRange &attribute_distances) {
		//std::cout << "set_attribute_distances \n";
		//boost::for_each(attribute_distance_range, print_attribute_distance());
		//_feature_distance.set_attribute_distances(attribute_distance_range);
		//_feature_distance = make_multidimensional_feature_distance<attribute_distance_type>(multidimensional_distance_type(), attribute_distances);
		fusioning.set_attribute_distances(attribute_distances);
	}

	template <class MultidimensionalDistance> void set_multidimensional_distance(const MultidimensionalDistance &multidimensional_distance) {
		fusioning.set_multidimensional_distance(multidimensional_distance);
	}

	template <class ZoneDistance> void set_zone_distance(const ZoneDistance &zone_distance) {
		fusioning.set_zone_distance(zone_distance);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void start() {}

	fusion_map_range_type get_fusion_maps(size_t begin = 0, bool compute_zones = false) {
		//return make_fusion_map_range(zone_fusions, begin, zone_fusions.size(), util::make_ref_range(voronoi_zones), compute_zones);
		return fusioning.get_fusion_maps(voronoi_zones, begin, compute_zones);
	}

	size_t get_fusion_size() const {
		return fusioning.get_fusion_size();
	}

	size_t get_feature_size() const {
		return boost::size(features);
	}

	size_t get_unique_feature_size() const {
		return boost::size(unique_features);
	}

	size_t get_bounded_feature_size() const {
		return boost::size(bounded_features);
	}

	reverse_fusion_map_range_type get_reverse_fusion_maps(size_t begin = 0, bool compute_zones = false) {
		return boost::adaptors::reverse(get_fusion_maps(begin, compute_zones));
	}

private:
	feature_container_type features;
	boundary_type boundary;
	feature_range_type unique_features;
	feature_range_type bounded_features;
	voronoi_zones_type voronoi_zones;
	voronoi_map_type voronoi_map;
	neighborhood_type neighborhood;
	zone_neighbor_container_type zone_neighbors;
	zone_neighbor_container_type filtered_zone_neighbors;
	fusioning_type fusioning;

	bool has_boundary() {
		return !boundary.is_empty();
	}

	bool has_features() {
		return !features.empty();
	}

	/*template <class FeatureRange> bool is_valid_features(const FeatureRange &features) {
		return !has_features() || (boost::distance(this->features) == boost::distance(features));
	}*/

	/*template <class FeatureRange> void assign_features(const FeatureRange &features) {
		util::assign(this->features, features);
	}*/

	struct print_geometry {

		void  operator()(const Feature &feature) const {
			std::cout << feature.get_geometry() << std::endl;
		}
	};

	struct print_id {

		void  operator()(const Feature &feature) const {
			std::cout << feature.get_id() << std::endl;
		}
	};

	struct is_feature_bounded {

		is_feature_bounded(const boundary_type &boundary) : boundary(boundary) {}

		bool operator()(const Feature &feature) const {
			return CGAL::bounded_side_2(boundary.vertices_begin(), boundary.vertices_end(), feature.get_geometry()) != CGAL::ON_UNBOUNDED_SIDE;
		}

		const boundary_type &boundary;
	};

	template <class EdgeToZoneNeighbor, class Container> struct edge_to_zone_neighbor_output {

		EdgeToZoneNeighbor edge_to_zone_neihbor;
		Container &container;

		edge_to_zone_neighbor_output(const EdgeToZoneNeighbor &edge_to_zone_neihbor, Container &container) : edge_to_zone_neihbor(edge_to_zone_neihbor), container(container) {}

		template <class Edge> void operator() (const Edge &edge) const {
			container.push_back(edge_to_zone_neihbor(edge));
		}
	};

	template <class EdgeToZoneNeighbor, class Container> inline auto make_edge_to_zone_neighbor_output(const EdgeToZoneNeighbor &edge_to_zone_neighbor, Container &container) {
		return boost::make_function_output_iterator(edge_to_zone_neighbor_output<EdgeToZoneNeighbor, Container>(edge_to_zone_neighbor, container));
	}

	template <class RetainedContainer, class FilteredContainer, class EdgeToZoneNeighbor> void initialize_zone_neighbors_with_finite_edges(RetainedContainer &retained, FilteredContainer &filtered, const EdgeToZoneNeighbor &edge_to_zone_neihbor) {
		auto zone_neighboring_filter = boost::adaptors::filtered(make_edge_to_zone_adaptor(zone_neighboring()));
		auto retained_output = make_edge_to_zone_neighbor_output(edge_to_zone_neihbor, retained);
		auto filtered_output = make_edge_to_zone_neighbor_output(edge_to_zone_neihbor, filtered);
		boost::algorithm::partition_copy(voronoi_map.get_finite_edges() | zone_neighboring_filter, retained_output, filtered_output, make_edge_to_zone_adaptor(neighborhood));
	}

	template <class RetainedContainer, class FilteredContainer> void initialize_zone_neighbors_with_finite_edges(RetainedContainer &retained, FilteredContainer &filtered) {
		initialize_zone_neighbors_with_finite_edges(retained, filtered, make_edge_to_zone_adaptor(zone_neighbor_maker<zone_type>()));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct join_zone_fusion {

		void operator()(zone_fusion_type &zone_fusion) const {
			CGAL::Polygon_with_holes_2<Kernel> join_result;
			CGAL::join(zone_fusion.get_zone1().get_geometry(), zone_fusion.get_zone2().get_geometry(), join_result);
			zone_fusion.get_fusion().set_geometry(join_result);
		}
	};

	struct join_fusion_map {

		template <class FusionStep> void operator()(FusionStep &fusion_map) {
			//adobe::timer_t timer;
			fusion_map.compute_zones();
			//timer.report("joining");
		}
	};

	struct join_fusion_map2 {

		template <class FusionStep> void operator()(FusionStep fusion_map) {
			//adobe::timer_t timer;
			fusion_map.compute_zones();
			//timer.report("joining");
		}
	};

	bool is_valid_boundary(const boundary_type &boundary) {
		return !boundary.is_empty() && is_valid_polygon(boundary);
	}


	struct step_timing {

		struct get_geometry {

			template <class Zone> void operator()(const Zone &zone) {
				zone.get_geometry();
			}
		};

		template <class Step> void operator()(const Step &step) {
			//adobe::timer_t timer;
			boost::for_each(step.zone_range(), get_geometry());
			//timer.report("step");
		}
	};
};

} // namespace geofis

#endif /* ZONING_HPP_ */
