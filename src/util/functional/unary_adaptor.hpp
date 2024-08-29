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
#ifndef UNARY_ADAPTOR_HPP_
#define UNARY_ADAPTOR_HPP_

#include <util/functional/function_adaptor_traits.hpp>

namespace util {

template <class Unary> struct unary_adaptor_traits {

	typedef typename boost::remove_const<Unary>::type unary_type;
	typedef typename boost::is_variant<unary_type>::type is_variant_type;
	typedef typename function_adaptor_traits<Unary>::result_type result_type;
};

template <class Unary, class IsVariant = typename unary_adaptor_traits<Unary>::is_variant_type> struct unary_adaptor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Unary> struct unary_adaptor<Unary, boost::false_type> {

	typedef typename unary_adaptor_traits<Unary>::result_type result_type;

	unary_adaptor() {}
	unary_adaptor(const Unary &unary) : unary(unary) {}
	template <BOOST_VARIANT_ENUM_PARAMS(class T)> unary_adaptor(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &unary) : unary(boost::get<Unary>(unary)) {}

	template <class T> result_type operator()(T &x) const {
		return unary(x);
	}

	template <class T> result_type operator()(const T &x) const {
		return unary(x);
	}

	Unary unary;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VariantUnary> struct unary_adaptor<VariantUnary, boost::true_type> {

	typedef typename unary_adaptor_traits<VariantUnary>::result_type result_type;

	unary_adaptor() {}
	unary_adaptor(const VariantUnary &variant_unary) : variant_unary(variant_unary) {}

	template <class T> struct unary_visitor : public boost::static_visitor<result_type> {

		unary_visitor(T &x) : x(x) {}

	    template <class Unary> result_type operator()(const Unary &unary) const {
	    	return unary(x);
	    }

		T &x;
	};

	template <class T> result_type operator()(T &x) const {
		return boost::apply_visitor(unary_visitor<T>(x), variant_unary);
	}

	template <class T> result_type operator()(const T &x) const {
		return boost::apply_visitor(unary_visitor<const T>(x), variant_unary);
	}

	VariantUnary variant_unary;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Unary> unary_adaptor<Unary> make_unary_adaptor(const Unary &unary) {
	return unary_adaptor<Unary>(unary);
}

} // namespace util

#endif /* UNARY_ADAPTOR_HPP_ */
