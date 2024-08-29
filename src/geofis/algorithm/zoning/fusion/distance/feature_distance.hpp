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
#ifndef FEATURE_DISTANCE_HPP_
#define FEATURE_DISTANCE_HPP_

#include <vector>
#include <util/assert.hpp>
#include <util/functional/unary_adaptor.hpp>
#include <util/functional/binary_adaptor.hpp>
#include <util/range/zipped_with_range.hpp>
#include <boost/range.hpp>
#include <geofis/data/is_monodimensional_feature.hpp>

namespace geofis {

/*
@startuml

title feature_distance class diagram

class feature_distance<MultidimensionalDistance, AttributeDistance> {
- multidimensional_distance : MultidimensionalDistance
- attribute_distances : AttributeDistance [1..*]
+ operator()<Feature> (lhs: Feature, rhs: Feature) : double
}

class multidimensional_feature_distance<MultidimensionalDistance> {
- multidimensional_distance : MultidimensionalDistance
+ operator()<Feature> (lhs: Feature, rhs: Feature) : double
}

feature_distance <|.down. multidimensional_feature_distance: <<bind>> \n <AttributeDistance -> void>

class monodimensional_feature_distance<AttributeDistance> {
- attribute_distance : AttributeDistance
+ operator()<Feature> (lhs: Feature, rhs: Feature) : double
}

feature_distance <|.down. monodimensional_feature_distance: <<bind>> \n <MultidimensionalDistance -> void>

@enduml
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class MultidimensionalDistance, class AttributeDistance> class feature_distance {

public:
	typedef MultidimensionalDistance multidimensional_distance_type;
	typedef AttributeDistance attribute_distance_type;
	typedef typename util::unary_adaptor_traits<MultidimensionalDistance>::result_type result_type;

	feature_distance() {}
	feature_distance(int n) : attribute_distances(n) {}
	feature_distance(int n, const AttributeDistance &attribute_distance) : attribute_distances(n, attribute_distance) {}
	template <class AttributeDistanceRange> feature_distance(const AttributeDistanceRange &attribute_distances) : attribute_distances(boost::begin(attribute_distances), boost::end(attribute_distances)) {}
	template <class AttributeDistanceRange> feature_distance(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) : multidimensional_distance(multidimensional_distance), attribute_distances(boost::begin(attribute_distances), boost::end(attribute_distances)) {}
	feature_distance(const MultidimensionalDistance &multidimensional_distance, int n) : multidimensional_distance(multidimensional_distance), attribute_distances(n) {}

	struct attribute_distance_computer {

		typedef typename util::binary_adaptor_traits<AttributeDistance>::result_type result_type;

		template <class Attribute> result_type operator()(const util::binary_adaptor<AttributeDistance> &attribute_distance, const Attribute &lhs, const Attribute &rhs) const {
			return attribute_distance(lhs, rhs);
		}
	};

	template <class Feature> result_type operator()(const Feature &lhs, const Feature &rhs) const {
		UTIL_REQUIRE(lhs.get_normalized_attribute_size() == attribute_distances.size());
		UTIL_REQUIRE(lhs.get_normalized_attribute_size() == rhs.get_normalized_attribute_size());
		return multidimensional_distance(util::make_zipped_with(attribute_distance_computer(), attribute_distances, lhs.get_normalized_attribute_range(), rhs.get_normalized_attribute_range()));
	}

private:
	util::unary_adaptor<MultidimensionalDistance> multidimensional_distance;
	std::vector<util::binary_adaptor<AttributeDistance> > attribute_distances;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_distance for multidimensional distance

template <class MultidimensionalDistance> class feature_distance<MultidimensionalDistance, void> {

public:
	typedef MultidimensionalDistance multidimensional_distance_type;
	typedef typename util::binary_adaptor_traits<MultidimensionalDistance>::result_type result_type;

	feature_distance() {}
	feature_distance(const MultidimensionalDistance &multidimensional_distance) : multidimensional_distance(multidimensional_distance) {}

	template <class Feature> result_type operator()(const Feature &lhs, const Feature &rhs) const {
		return multidimensional_distance(lhs.get_normalized_attribute_range(), rhs.get_normalized_attribute_range());
	}

	util::binary_adaptor<MultidimensionalDistance> multidimensional_distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <class AttributeDistance, class Feature, class IsMonodimensionalFeature = typename is_monodimensional_feature<Feature>::type> struct monodimensional_feature_distance_computer;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_distance_computer for monodimensional feature

template <class AttributeDistance, class Feature> struct monodimensional_feature_distance_computer<AttributeDistance, Feature, boost::true_type> {

	typedef typename util::function_adaptor_traits<AttributeDistance>::result_type result_type;

	monodimensional_feature_distance_computer(const AttributeDistance &attribute_distance) : attribute_distance(attribute_distance) {}

	result_type operator()(const Feature &lhs, const Feature &rhs) const {
		return attribute_distance(lhs.get_normalized_attribute(), rhs.get_normalized_attribute());
	}

	const AttributeDistance &attribute_distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_distance_computer for multidimensional feature

template <class AttributeDistance, class Feature> struct monodimensional_feature_distance_computer<AttributeDistance, Feature, boost::false_type> {

	typedef typename util::function_adaptor_traits<AttributeDistance>::result_type result_type;

	monodimensional_feature_distance_computer(const AttributeDistance &attribute_distance) : attribute_distance(attribute_distance) {}

	result_type operator()(const Feature &lhs, const Feature &rhs) const {
		UTIL_REQUIRE(lhs.get_normalized_attribute_size() == 1);
		UTIL_REQUIRE(rhs.get_normalized_attribute_size() == 1);
		return attribute_distance(lhs.get_normalized_attribute_range().front(), rhs.get_normalized_attribute_range().front());
	}

	const AttributeDistance &attribute_distance;
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_distance for monodimensional distance

template <class AttributeDistance> class feature_distance<void, AttributeDistance> {

public:
	typedef AttributeDistance attribute_distance_type;
	typedef typename util::binary_adaptor_traits<AttributeDistance>::result_type result_type;
	typedef util::binary_adaptor<AttributeDistance> attribute_distance_adaptor_type;

	feature_distance() {}
	feature_distance(const AttributeDistance &attribute_distance) : attribute_distance(attribute_distance) {}
	template <class OtherAttributeDistance> feature_distance(const OtherAttributeDistance &attribute_distance) : attribute_distance(attribute_distance) {}

	template <class Feature> result_type operator()(const Feature &lhs, const Feature &rhs) const {
		return detail::monodimensional_feature_distance_computer<attribute_distance_adaptor_type, Feature>(attribute_distance)(lhs, rhs);
	}

private:
	attribute_distance_adaptor_type attribute_distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class MultidimensionalDistance> feature_distance<MultidimensionalDistance, void> make_multidimensional_feature_distance(const MultidimensionalDistance &multidimensional_distance) {
	return feature_distance<MultidimensionalDistance, void>(multidimensional_distance);
}

template <class AttributeDistance, class MultidimensionalDistance, class AttributeDistanceRange> class feature_distance<MultidimensionalDistance, AttributeDistance> make_multidimensional_feature_distance(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) {
	return feature_distance<MultidimensionalDistance, AttributeDistance>(multidimensional_distance, attribute_distances);
}

template <class AttributeDistance, class OtherAttributeDistance> feature_distance<void, AttributeDistance> make_monodimensional_feature_distance(const OtherAttributeDistance &attribute_distance) {
	return feature_distance<void, AttributeDistance>(attribute_distance);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureDistance> struct feature_distance_maker;

template <class MultidimensionalDistance> struct feature_distance_maker<feature_distance<MultidimensionalDistance, void> > {

	template <class AttributeDistanceRange> feature_distance<MultidimensionalDistance, void> operator()(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) const {
		return make_multidimensional_feature_distance(multidimensional_distance);
	}
};

template <class MultidimensionalDistance, class AttributeDistance> struct feature_distance_maker<feature_distance<MultidimensionalDistance, AttributeDistance> > {

	template <class AttributeDistanceRange> feature_distance<MultidimensionalDistance, AttributeDistance> operator()(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) const {
		return make_multidimensional_feature_distance<AttributeDistance>(multidimensional_distance, attribute_distances);
	}
};

template <class AttributeDistance> struct feature_distance_maker<feature_distance<void, AttributeDistance> > {

	template <class MultidimensionalDistance, class AttributeDistanceRange> feature_distance<void, AttributeDistance> operator()(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) {
		UTIL_REQUIRE(boost::size(attribute_distances) == 1);
		return make_monodimensional_feature_distance<AttributeDistance>(*boost::begin(attribute_distances));
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureDistance, class MultidimensionalDistance, class AttributeDistanceRange> FeatureDistance make_feature_distance(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) {
	return feature_distance_maker<FeatureDistance>()(multidimensional_distance, attribute_distances);
}

} // namespace geofis

#endif /* FEATURE_DISTANCE_HPP_ */
