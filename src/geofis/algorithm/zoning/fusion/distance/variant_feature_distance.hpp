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
#ifndef VARIANT_FEATURE_DISTANCE_HPP_
#define VARIANT_FEATURE_DISTANCE_HPP_

#include <boost/variant.hpp>
#include <geofis/algorithm/zoning/fusion/distance/feature_distance.hpp>

#ifdef ihoi
#include <iostream>
#include <geofis/algorithm/zoning/fusion/distance/variant_multidimensional_distance.hpp>
#include <geofis/algorithm/zoning/fusion/distance/variant_attribute_distance.hpp>

namespace boost {

// boost variant optimizations

template <> struct has_nothrow_copy<geofis::feature_distance<geofis::variant_multidimensional_distance, geofis::variant_attribute_distance> > : true_type {};
//template <> struct has_nothrow_constructor<util::euclidean_range_distance> : false_type {};
//template <> struct has_nothrow_assign<util::euclidean_range_distance> : false_type {};
//template <> struct has_trivial_constructor<util::euclidean_range_distance> : true_type {};
//template <> struct has_trivial_copy<util::euclidean_range_distance> : true_type {};
//template <> struct has_trivial_assign<util::euclidean_range_distance> : true_type {};


//template <> struct has_nothrow_copy<util::minkowski_range_distance> : false_type {};
//template <> struct has_nothrow_constructor<util::minkowski_range_distance> : false_type {};
//template <> struct has_nothrow_assign<util::minkowski_range_distance> : false_type {};
//template <> struct has_trivial_constructor<util::minkowski_range_distance> : true_type {};
//template <> struct has_trivial_copy<util::minkowski_range_distance> : true_type {};
//template <> struct has_trivial_assign<util::minkowski_range_distance> : true_type {};

namespace detail { namespace variant {

template <typename Visitor, typename VoidPtrCV/*, class MultidimensionalDistance, class AttributeDistance*/> /*inline*/ __attribute__((always_inline)) BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type) visitation_impl_invoke_impl(int, Visitor &visitor, VoidPtrCV storage, geofis::feature_distance</*MultidimensionalDistance, AttributeDistance*/geofis::variant_multidimensional_distance, geofis::variant_attribute_distance> *, mpl::true_)
{
	//std::cout << "visitation_impl_invoke_impl mpl::true_\n";
    //return visitor.internal_visit(cast_storage<geofis::feature_distance<MultidimensionalDistance, AttributeDistance> >(storage), 1L);
    return visitor.internal_visit(cast_storage<geofis::feature_distance<geofis::variant_multidimensional_distance, geofis::variant_attribute_distance> >(storage), 1L);
}

template <typename Visitor, typename VoidPtrCV/*, class MultidimensionalDistance, class AttributeDistance*/> /*inline*/ __attribute__((always_inline)) BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type) visitation_impl_invoke_impl(int, Visitor &visitor, VoidPtrCV storage, geofis::feature_distance</*MultidimensionalDistance, AttributeDistance*/geofis::variant_multidimensional_distance, geofis::variant_attribute_distance> *, mpl::false_)
{
	//std::cout << "visitation_impl_invoke_impl mpl::false_\n";
    //return visitor.internal_visit(cast_storage<geofis::feature_distance<MultidimensionalDistance, AttributeDistance> >(storage), 1L);
    return visitor.internal_visit(cast_storage<geofis::feature_distance<geofis::variant_multidimensional_distance, geofis::variant_attribute_distance> >(storage), 1L);
}


//template <> struct has_nothrow_move_constructor<util::euclidean_range_distance> : false_type {};
//template <> struct has_nothrow_move_constructor<util::minkowski_range_distance> : false_type {};

}}

}  // namespace boost
#endif

namespace geofis {

template <class MultidimensionalDistance, class AttributeDistance> struct variant_feature_distance_traits {

	typedef feature_distance<MultidimensionalDistance, AttributeDistance> 									multidimensional_feature_distance_type;
	typedef feature_distance<void, AttributeDistance> 														monodimensional_feature_distance_type;
	typedef boost::variant<multidimensional_feature_distance_type, monodimensional_feature_distance_type> 	variant_feature_distance_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T0, class T1> struct feature_distance_maker<boost::variant<T0, T1> > {

	typedef T0 multidimensional_feature_distance_type;
	typedef T1 monodimensional_feature_distance_type;
	typedef boost::variant<multidimensional_feature_distance_type, monodimensional_feature_distance_type> variant_feature_distance_type;

	template <class MultidimensionalDistance, class AttributeDistanceRange> variant_feature_distance_type make_monodimensional_feature_distance(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) const {
		return make_feature_distance<monodimensional_feature_distance_type>(multidimensional_distance, attribute_distances);
	}

	template <class MultidimensionalDistance, class AttributeDistanceRange> variant_feature_distance_type make_multidimensional_feature_distance(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) const {
		return make_feature_distance<multidimensional_feature_distance_type>(multidimensional_distance, attribute_distances);
	}

	template <class MultidimensionalDistance, class AttributeDistanceRange> variant_feature_distance_type operator()(const MultidimensionalDistance &multidimensional_distance, const AttributeDistanceRange &attribute_distances) const {
		return boost::size(attribute_distances) == 1 ? make_monodimensional_feature_distance(multidimensional_distance, attribute_distances) : make_multidimensional_feature_distance(multidimensional_distance, attribute_distances);
	}
};

} // namespace geofis

#endif /* VARIANT_FEATURE_DISTANCE_HPP_ */
