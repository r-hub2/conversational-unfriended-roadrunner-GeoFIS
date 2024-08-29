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
#ifndef FUNCTION_ADAPTOR_TRAITS_HPP_
#define FUNCTION_ADAPTOR_TRAITS_HPP_

#include <boost/utility/result_of.hpp>
#include <boost/variant/is_variant.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace util {

template <class Function, class IsVariant = typename boost::is_variant<typename boost::remove_const<Function>::type>::type> struct function_adaptor_traits;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Function> struct function_adaptor_traits<Function, boost::false_type> {

	typedef typename Function::result_type result_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VariantFunction> struct function_adaptor_traits<VariantFunction, boost::true_type> {

	typedef typename VariantFunction::types types;
	typedef typename boost::mpl::front<types>::type front_type;
	typedef typename front_type::result_type result_type;

	template <class T> struct is_result_type {

		typedef typename boost::is_same<result_type, typename T::result_type>::type type;
	};

	BOOST_MPL_ASSERT((boost::mpl::accumulate<types, boost::mpl::true_, boost::mpl::and_<is_result_type<boost::mpl::_2>, boost::mpl::_1> >));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VariantFunction> struct function_adaptor_traits<const VariantFunction, boost::true_type> : function_adaptor_traits<VariantFunction, boost::true_type> {};

} // namespace util

#endif /* FUNCTION_ADAPTOR_TRAITS_HPP_ */
