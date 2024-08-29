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
#ifndef ATTRIBUTE_RANGE_HPP_
#define ATTRIBUTE_RANGE_HPP_

#include <boost/range/adaptor/transformed.hpp>
#include <geofis/data/is_monodimensional_feature.hpp>

namespace geofis {

namespace detail {

template <class Feature, class IsMonodimensionalFeature = typename is_monodimensional_feature<Feature>::type> struct attribute_function;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of attribute_function for monodimensional feature

template <class Feature> struct attribute_function<Feature, boost::true_type> {

	typedef typename Feature::attribute_type result_type;

	result_type operator() (const Feature &feature) const { return feature.get_attribute(); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of attribute_function for multidimensional feature

template <class Feature> struct attribute_function<Feature, boost::false_type> {

	typedef typename Feature::const_attribute_range_type result_type;

	result_type operator() (const Feature &feature) const { return feature.get_attribute_range(); }
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureRange> struct attribute_range_traits {

	typedef typename boost::range_value<FeatureRange>::type feature_type;
	typedef boost::transformed_range<detail::attribute_function<feature_type>, FeatureRange> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureRange> class attribute_range : public attribute_range_traits<FeatureRange>::base_type {

	typedef typename attribute_range_traits<FeatureRange>::base_type base_type;
	typedef typename attribute_range_traits<FeatureRange>::feature_type feature_type;

public:
	attribute_range(FeatureRange &features) : base_type(detail::attribute_function<feature_type>(), features) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureRange> attribute_range<const FeatureRange> make_attribute_range(const FeatureRange &features) {
	return attribute_range<const FeatureRange>(features);
}

} // namespace geofis

#endif /* ATTRIBUTE_RANGE_HPP_ */
