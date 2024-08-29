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
#ifndef VORONOI_ZONE_HPP_
#define VORONOI_ZONE_HPP_

#include <boost/ref.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace geofis {

/*
@startuml

title voronoi_zone class diagram

class voronoi_zone<Geometry, Feature> {
- geometry : Geometry
+ get_id() : id_type
}

hide voronoi_zone methods
show voronoi_zone methods

interface Feature<Id> {
+ get_id() : Id
+ get_attribute_range() : const_attribute_range_type
}

hide Feature members
show Feature methods

voronoi_zone o-right- Feature

@enduml
*/

template <class Geometry, class Feature> class voronoi_zone {

	typedef boost::reference_wrapper<const Feature> feature_reference_type;

public:
	typedef Geometry geometry_type;
	typedef Feature feature_type;
	typedef typename Feature::id_type id_type;
	typedef typename Feature::const_attribute_range_type const_attribute_range_type;

	voronoi_zone(const feature_type &feature) : feature(feature) {}

	id_type get_id() const {
		return feature.get().get_id();
	}

	geometry_type get_geometry() const {
		return geometry;
	}

	void set_geometry(const geometry_type &geometry) {
		this->geometry = geometry;
	}

	const feature_type &get_feature() const {
		return feature;
	}

	const_attribute_range_type get_attribute_range() const {
		return boost::unwrap_ref(feature).get_attribute_range();
	}

private:
	feature_reference_type feature;
	geometry_type geometry;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class Feature> struct voronoi_zone_builder {

	typedef voronoi_zone<Geometry, Feature> result_type;

	result_type operator() (const Feature &feature) const {
		return result_type(feature);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class FeatureRange> struct voronoi_zone_range_traits {

	typedef typename boost::range_value<FeatureRange>::type feature_type;
	typedef voronoi_zone_builder<Geometry, feature_type> voronoi_zone_builder_type;
	typedef boost::transformed_range<voronoi_zone_builder_type, FeatureRange> voronoi_zone_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class FeatureRange> inline typename voronoi_zone_range_traits<Geometry, const FeatureRange>::voronoi_zone_range_type make_voronoi_zone_range(const FeatureRange &features) {

	typedef typename voronoi_zone_range_traits<Geometry, const FeatureRange>::voronoi_zone_builder_type voronoi_zone_builder_type;

	return boost::adaptors::transform(features, voronoi_zone_builder_type());
}

} // namespace geofis

#endif /* VORONOI_ZONE_HPP_ */
