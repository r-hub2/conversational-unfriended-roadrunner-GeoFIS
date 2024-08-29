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
#ifndef ZONE2_HPP_
#define ZONE2_HPP_

#include <vector>
#include <cmath>
#include <util/assert.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/range/algorithm_ext/for_each.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <CGAL/Boolean_set_operations_2.h>
#include <util/range/unwrap_range.hpp>
#include <util/iterator/output/back_insert_reference_iterator.hpp>
#include <geofis/data/featurable.hpp>
#include <geofis/geometry/geometrical.hpp>
#include <geofis/geometry/area/geometry_area.hpp>
#include <geofis/identifiable/identifiable_comparator.hpp>

namespace geofis {

struct zone_attributes_computer;
template <class ZoneFusion> class zone_geometry_computer;
template <class Zone> class zone_area_computer;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Feature> struct accumulator_extractor {

	typedef double result_type;
	typedef boost::accumulators::extractor<Feature> extractor_type;

	extractor_type extractor;

	accumulator_extractor(const extractor_type &extractor) : extractor(extractor) {}

	template <class Accumulator> double operator() (const Accumulator &accumulator) const {
		return extractor(accumulator);
	}
};

template <class Feature> accumulator_extractor<Feature> inline make_accumulator_extractor(const boost::accumulators::extractor<Feature> extractor) {
	return accumulator_extractor<Feature>(extractor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct standard_deviation_extractor {

	typedef double result_type;
	typedef boost::accumulators::extractor<boost::accumulators::tag::variance> extractor_type;

	extractor_type extractor;

	template <class Accumulator> double operator() (const Accumulator &accumulator) const {
		return std::sqrt(extractor(accumulator));
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
@startuml

title zone class diagram

skinparam linetype ortho

interface VoronoiZone {
	+ get_attribute_range() : const_attribute_range_type
	+ get_geometry() : geometry_type
}

package "boost::accumulators" {

	class accumulator_set<Sample, Features>
}

class accumulator_type <<(T,#FF7700) typedef>>

accumulator_set <|.down. accumulator_type : <<bind>> \n <Sample -> double> \n <Features -> features<tag::variance>>

class zone<Geometry, VoronoiZone> {
	- geometry : boost::optional<Geometry>
	- area : boost::optional<double>
	+ get_id() : id_type
	+ set_geometry(Geometry)
	+ get_geometry() : Geometry
	+ size() : size_t
	+ get_area() : double
	+ get_means() : mean_range_type
	+ get_mean(size_t) : double
	+ get_variances() : variance_range_type
	+ get_variance(size_t) : double
	+ get_standard_deviations() : standard_deviation_range_type
	+ get_standard_deviation(size_t) : double
	+ merge(Zone)
	- set_area(area)
	- has_geometry() : bool
	- compute_geometry()
	- has_attributes() : bool
	- compute_attributes()
}

note right
  **area optimisation**
  area is optional, determined from geometry if absent
  set_area() is private, called by friend class zone_area_computer
end note

zone o-right- "1..n\nattribute_accumulators" accumulator_type
zone o-down- "1..n\nvoronoi_zones" VoronoiZone

class zone_area_computer<Zone>
class zone_geometry_computer<ZoneFusion>
class zone_attributes_computer

zone <.right. zone_area_computer : <<friend>>
zone <.right. zone_geometry_computer : <<friend>>
zone <.right. zone_attributes_computer : <<friend>>

hide accumulator_type members
hide accumulator_set members
hide VoronoiZone members
show VoronoiZone methods
hide zone_area_computer members
hide zone_geometry_computer members
hide zone_attributes_computer members

@enduml
*/

template <class Geometry, class VoronoiZone> class zone {

	friend zone_attributes_computer;
	template <class ZoneFusion> friend class zone_geometry_computer;
	template <class Zone> friend class zone_area_computer;

public:
	typedef typename VoronoiZone::id_type id_type;
	typedef Geometry geometry_type;
	typedef VoronoiZone voronoi_zone_type;
	typedef typename VoronoiZone::feature_type feature_type;
	typedef boost::optional<double> area_type;

private:
	typedef boost::optional<geometry_type> optional_geometry_type;
	typedef boost::reference_wrapper<const voronoi_zone_type> voronoi_zone_reference_type;
	typedef std::vector<voronoi_zone_reference_type> voronoi_zone_reference_container_type;
	typedef boost::accumulators::accumulator_set<double, boost::accumulators::features<boost::accumulators::tag::variance> > accumulator_type;
	typedef std::vector<accumulator_type> accumulator_container_type;
	typedef accumulator_extractor<boost::accumulators::tag::mean> accumulator_extractor_mean_type;
	typedef accumulator_extractor<boost::accumulators::tag::variance> accumulator_extractor_variance_type;
	typedef boost::transformed_range<accumulator_extractor_mean_type, const accumulator_container_type> mean_range_type;
	typedef boost::transformed_range<accumulator_extractor_variance_type, const accumulator_container_type> variance_range_type;
	typedef boost::transformed_range<standard_deviation_extractor, const accumulator_container_type> standard_deviation_range_type;

public:
	typedef typename util::unwrap_range_traits<const voronoi_zone_reference_container_type>::unwrap_range_type voronoi_zone_range_type;
	typedef typename featurable_range_traits<const voronoi_zone_range_type>::feature_range_type feature_range_type;

	zone(const voronoi_zone_type &voronoi_zone) : id(voronoi_zone.get_id()), voronoi_zones(1, boost::cref(voronoi_zone)) {}
	template <class VoronoiZoneRange> zone(const VoronoiZoneRange &voronoi_zones) : voronoi_zones(boost::begin(voronoi_zones), boost::end(voronoi_zones)) {
		UTIL_REQUIRE(!get_voronoi_zones().empty());
		id = boost::min_element(get_voronoi_zones(), identifiable_comparator())->get_id();
	}

	id_type get_id() const {
		return id;
	}

	geometry_type get_geometry() const {
		const_cast<zone *>(this)->compute_geometry();
		return geometry.get();
	}

	void set_geometry(const geometry_type &geometry) {
		this->geometry = geometry;
	}

	const voronoi_zone_type &get_voronoi_zone(size_t index) const {
		return voronoi_zones.at(index);
	}

	voronoi_zone_range_type get_voronoi_zones() const {
		return util::make_unwrap_range(voronoi_zones);
	}

	const feature_type &get_feature(size_t index) const {
		return get_voronoi_zone(index).get_feature();
	}

	feature_range_type get_feature_range() const {
		return make_feature_range(get_voronoi_zones());
	}

	size_t size() const {
		return voronoi_zones.size();
	}

	double get_area() const {
		if(!area) {
			const_cast<zone *>(this)->compute_geometry();
			const_cast<zone *>(this)->area = get_double_geometry_area(geometry.get());
		}
		return area.get();
	}

	double get_mean(size_t pos) const {
		const_cast<zone *>(this)->compute_attributes();
		return extract(boost::accumulators::mean, pos);
	}

	double get_variance(size_t pos) const {
		const_cast<zone *>(this)->compute_attributes();
		return extract(boost::accumulators::variance, pos);
	}

	double get_standard_deviation(size_t pos) const {
		const_cast<zone *>(this)->compute_attributes();
		return std::sqrt(get_variance(pos));
	}

	mean_range_type get_means() const {
		const_cast<zone *>(this)->compute_attributes();
		return boost::adaptors::transform(attribute_accumulators, make_accumulator_extractor(boost::accumulators::mean));
	}

	variance_range_type get_variances() const {
		const_cast<zone *>(this)->compute_attributes();
		return boost::adaptors::transform(attribute_accumulators, make_accumulator_extractor(boost::accumulators::variance));
	}

	standard_deviation_range_type get_standard_deviations() const {
		const_cast<zone *>(this)->compute_attributes();
		return boost::adaptors::transform(attribute_accumulators, standard_deviation_extractor());
	}

	void merge(const zone &merged_zone) {
		boost::copy(merged_zone.get_voronoi_zones(), util::back_inserter_reference(voronoi_zones));
		id = boost::min_element(get_voronoi_zones(), identifiable_comparator())->get_id();
		if(area)
			area = area.get() + merged_zone.get_area();
		geometry = boost::none;
		attribute_accumulators.clear();
	}

private:
	id_type id;
	area_type area;
	voronoi_zone_reference_container_type voronoi_zones;
	optional_geometry_type geometry;
	accumulator_container_type attribute_accumulators;

	void set_area(double area) {
		this->area = area;
	}

	bool has_attributes() const {
		return !attribute_accumulators.empty();
	}

	void compute_attributes() {
		if(attribute_accumulators.empty())
			accumulate_voronoi_zones();
	}

	bool has_geometry() const { return geometry.is_initialized(); }

	void compute_geometry() {
		if(!geometry)
			compute_geometry(make_geometry_range(get_voronoi_zones()));
	}

	template <class Extractor> double extract(const Extractor &extractor, size_t pos) const {
		return extract(extractor, attribute_accumulators.at(pos));
	}

	template <class Extractor> double extract(const Extractor &extractor, const accumulator_type &accumulator) const {
		return extractor(accumulator);
	}

	template <class GeometryRange> void compute_geometry(const GeometryRange &geometries) {
		std::vector<geometry_type> result_join_geometries;
		CGAL::join(boost::begin(geometries), boost::end(geometries), std::back_inserter(result_join_geometries));
		UTIL_ASSERT(result_join_geometries.size() == 1);
		geometry = result_join_geometries[0];
	}

	struct make_accumulator {

		typedef accumulator_type result_type;

		result_type operator()(double attribute) const {
			accumulator_type accumulator;
			accumulator(attribute);
			return accumulator;
		}
	};

	struct accumulate_attribute {

		void operator()(accumulator_type &accumulator, double attribute) const {
			accumulator(attribute);
		}
	};

	struct accumulate_voronoi_zone {

		accumulate_voronoi_zone(accumulator_container_type &attribute_accumulators) : attribute_accumulators(attribute_accumulators) {}

		void operator()(const voronoi_zone_type &voronoi_zone) const {
			if(attribute_accumulators.empty())
				assign_attribute_accumulators(attribute_accumulators, voronoi_zone);
			else
				boost::for_each(attribute_accumulators, voronoi_zone.get_attribute_range(), accumulate_attribute());
		}

		void assign_attribute_accumulators(accumulator_container_type &attribute_accumulators, const voronoi_zone_type &voronoi_zone) const {
			boost::transform(voronoi_zone.get_attribute_range(), std::back_inserter(attribute_accumulators), make_accumulator());
		}

		accumulator_container_type &attribute_accumulators;
	};

	void accumulate_voronoi_zones() {
		boost::for_each(voronoi_zones, accumulate_voronoi_zone(attribute_accumulators));
	}
};

} // namespace geofis

#endif /* ZONE2_HPP_ */
