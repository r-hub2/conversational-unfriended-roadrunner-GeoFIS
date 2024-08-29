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
#ifndef FEATURE_NORMALIZATION_HPP_
#define FEATURE_NORMALIZATION_HPP_

#include <vector>
#include <boost/range.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION <= 107300
#include <boost/function_output_iterator.hpp>
#else
#include <boost/iterator/function_output_iterator.hpp>
#endif
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <util/algorithm/normalization.hpp>
#include <util/range/zipped_with_range.hpp>
#include <geofis/data/attribute_range.hpp>

namespace geofis {

template <class Feature, class IsMonodimensionalFeature = typename is_monodimensional_feature<Feature>::type> class feature_normalization;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <class FeatureIterator, class IsMonodimensionalFeature> struct feature_updater;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_updater for monodimensional feature

template <class FeatureIterator> struct feature_updater<FeatureIterator, boost::true_type> {

	typedef typename boost::iterator_value<FeatureIterator>::type feature_type;

	feature_updater(FeatureIterator feature) : feature(feature) {}

	template <class Attribute> void operator()(const Attribute &attribute) {
		update_feature(*feature++, attribute);
	}

	template <class Attribute> void update_feature(feature_type &feature, const Attribute &attribute) {

		BOOST_MPL_ASSERT((boost::is_same<typename feature_type::attribute_type, Attribute>));

		feature.set_normalized_attribute(attribute);
	}

	FeatureIterator feature;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_updater for multidimensional feature

template <class FeatureIterator> struct feature_updater<FeatureIterator, boost::false_type> {

	typedef typename boost::iterator_value<FeatureIterator>::type feature_type;

	feature_updater(FeatureIterator feature) : feature(feature) {}

	template <class Attribute> void operator()(const Attribute &attribute) {
		update_feature(*feature++, attribute);
	}

	template <class AttributeRange> void update_feature(feature_type &feature, const AttributeRange &attributes) {

		BOOST_MPL_ASSERT((boost::is_same<typename feature_type::attribute_type, typename boost::range_value<AttributeRange>::type>));

		feature.set_normalized_attribute_range(attributes);
	}

	FeatureIterator feature;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class IsMonodimensionalFeature, class FeatureIterator> inline feature_updater<FeatureIterator, IsMonodimensionalFeature> make_feature_updater(const FeatureIterator &feature) {
	return feature_updater<FeatureIterator, IsMonodimensionalFeature>(feature);
}

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_normalization for monodimensional feature

template <class Feature> class feature_normalization<Feature, boost::true_type> {

	typedef typename Feature::attribute_type attribute_type;

public:
	template <class FeatureRange> static feature_normalization initialize(const FeatureRange &features) {
		return feature_normalization(util::normalization<attribute_type>::initialize(make_attribute_range(features)));
	}

	template <class FeatureRange> void normalize(FeatureRange &features) const {
		normalize_to_function(features, detail::make_feature_updater<boost::true_type>(boost::begin(features)));
	}

	template <class FeatureRange, class UnaryFunction> void normalize_to_function(const FeatureRange &features, const UnaryFunction &function) const {
		normalize_to_output(features, boost::make_function_output_iterator(function));
	}

private:
	util::normalization<attribute_type> normalization;

	feature_normalization(const util::normalization<attribute_type> &normalization) : normalization(normalization) {}

	template <class FeatureRange, class OutputIterator> void normalize_to_output(const FeatureRange &features, OutputIterator output) const {
		normalization.normalize(make_attribute_range(features), output);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature_normalization for multidimensional feature

template <class Feature> class feature_normalization<Feature, boost::false_type> {

	typedef typename Feature::attribute_type attribute_type;

public:
	feature_normalization() {}

	template <class FeatureRange> static feature_normalization initialize(const FeatureRange &features) {
		UTIL_REQUIRE(!boost::empty(features));
		return initialize_with_attribute_range(make_attribute_range(features));
	}

	template <class FeatureRange> void normalize(FeatureRange &features) {
		normalize_to_function(features, detail::make_feature_updater<boost::false_type>(boost::begin(features)));
	}

	template <class FeatureRange, class UnaryFunction> void normalize_to_function(const FeatureRange &features, const UnaryFunction &function) const {
		normalize_to_output(features, boost::make_function_output_iterator(function));
	}

private:
	std::vector<util::normalize_function<attribute_type> > normalizers;

	template <class NormalizerRange> feature_normalization(const NormalizerRange &normalizers) : normalizers(boost::begin(normalizers), boost::end(normalizers)) {}

	struct update_min {

		template <class Tuple> void operator()(const Tuple &tuple) const {
			update(boost::get<0>(tuple), boost::get<1>(tuple));
		}

		void update(attribute_type &min, const attribute_type &attribute) const {
			min = std::min(min, attribute);
		}
	};

	struct update_max {

		template <class Tuple> void operator()(const Tuple &tuple) const {
			update(boost::get<0>(tuple), boost::get<1>(tuple));
		}

		void update(attribute_type &max, const attribute_type &attribute) const {
			max = std::max(max, attribute);
		}
	};

	struct attributes_mins_maxs {

		template <class AttributeRange2> attributes_mins_maxs(const AttributeRange2 &attributes) : mins(boost::begin(attributes), boost::end(attributes)), maxs(boost::begin(attributes), boost::end(attributes)) {
			BOOST_MPL_ASSERT((boost::is_same<attribute_type, typename boost::range_value<AttributeRange2>::type>));
		}

		template <class AttributeRange2> void operator()(const AttributeRange2 &attributes) {
			BOOST_MPL_ASSERT((boost::is_same<attribute_type, typename boost::range_value<AttributeRange2>::type>));
			boost::for_each(boost::combine(mins, attributes), update_min());
			boost::for_each(boost::combine(maxs, attributes), update_max());
		}

		std::vector<attribute_type> mins;
		std::vector<attribute_type> maxs;
	};

	template <class AttributeRange1> static feature_normalization initialize_with_attribute_range(const AttributeRange1 &attributes) {
		return initialize_with_attributes_mins_maxs(boost::for_each(attributes, attributes_mins_maxs(attributes.front())));
	}

	static feature_normalization initialize_with_attributes_mins_maxs(const attributes_mins_maxs &mins_maxs) {
		return feature_normalization(util::make_zipped_with(util::normalize_function_maker<attribute_type>(), mins_maxs.mins, mins_maxs.maxs));
	}

	struct attribute_normalizer {

		typedef attribute_type result_type;

		template <class Normalizer> result_type operator()(const Normalizer &normalizer, const attribute_type &attribute) const {
			return normalizer(attribute);
		}
	};

	template <class NormalizerRange> struct normalizer {

		typedef std::vector<attribute_type> result_type;

		normalizer(const NormalizerRange &normalizers) : normalizers(normalizers) {}

		template <class AttributeRange2> result_type operator()(const AttributeRange2 &attributes) const {
			UTIL_REQUIRE(boost::size(attributes) == boost::size(normalizers));
			return normalized(util::make_zipped_with(attribute_normalizer(), normalizers, attributes));
		}

		template <class Normalized> result_type normalized(const Normalized &normalized) const {
			return result_type(boost::begin(normalized), boost::end(normalized));
		}

		const NormalizerRange &normalizers;
	};

	template <class NormalizerRange> normalizer<NormalizerRange> make_normalizer(const NormalizerRange &normalizers) const {
		return normalizer<NormalizerRange>(normalizers);
	}

	template <class FeatureRange, class OutputIterator> void normalize_to_output(const FeatureRange &features, OutputIterator output) const {
		boost::transform(make_attribute_range(features), output, make_normalizer(normalizers));
	}
};

} // namespace geofis

#endif /* FEATURE_NORMALIZATION_HPP_ */
