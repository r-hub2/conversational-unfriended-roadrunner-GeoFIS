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
#ifndef BINARY_ADAPTOR_HPP_
#define BINARY_ADAPTOR_HPP_

#include <util/functional/function_adaptor_traits.hpp>

namespace util {

template <class Binary> struct binary_adaptor_traits {

	typedef typename boost::remove_const<Binary>::type binary_type;
	typedef typename boost::is_variant<binary_type>::type is_variant_type;
	typedef typename function_adaptor_traits<Binary>::result_type result_type;
};

template <class Binary, class IsVariant = typename binary_adaptor_traits<Binary>::is_variant_type> struct binary_adaptor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary> struct binary_adaptor<Binary, boost::false_type> {

	typedef typename binary_adaptor_traits<Binary>::result_type result_type;

	binary_adaptor() {}
	binary_adaptor(const Binary &binary) : binary(binary) {}
	template <BOOST_VARIANT_ENUM_PARAMS(class T)> binary_adaptor(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &binary) : binary(boost::get<Binary>(binary)) {}

	template <class T1, class T2> result_type operator()(T1 &lhs, T2 &rhs) const {
		return binary(lhs, rhs);
	}

	template <class T1, class T2> result_type operator()(const T1 &lhs, T2 &rhs) const {
		return binary(lhs, rhs);
	}

	template <class T1, class T2> result_type operator()(T1 &lhs, const T2 &rhs) const {
		return binary(lhs, rhs);
	}

	template <class T1, class T2> result_type operator()(const T1 &lhs, const T2 &rhs) const {
		return binary(lhs, rhs);
	}

	Binary binary;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VariantBinary> struct binary_adaptor<VariantBinary, boost::true_type> {

	typedef typename binary_adaptor_traits<VariantBinary>::result_type result_type;

	binary_adaptor() {}
	binary_adaptor(const VariantBinary &variant_binary) : variant_binary(variant_binary) {}

	template <class T1, class T2> struct binary_visitor : public boost::static_visitor<result_type> {

		binary_visitor(T1 &lhs, T2 &rhs) : lhs(lhs), rhs(rhs) {}

	    template <class Binary> result_type operator()(const Binary &binary) const {
	    	return binary(lhs, rhs);
	    }

		T1 &lhs;
		T2 &rhs;
	};

	template <class T1, class T2> result_type operator()(T1 &lhs, T2 &rhs) const {
		return boost::apply_visitor(binary_visitor<T1, T2>(lhs, rhs), variant_binary);
	}

	template <class T1, class T2> result_type operator()(const T1 &lhs, T2 &rhs) const {
		return boost::apply_visitor(binary_visitor<const T1, T2>(lhs, rhs), variant_binary);
	}

	template <class T1, class T2> result_type operator()(T1 &lhs, const T2 &rhs) const {
		return boost::apply_visitor(binary_visitor<T1, const T2>(lhs, rhs), variant_binary);
	}

	template <class T1, class T2> result_type operator()(const T1 &lhs, const T2 &rhs) const {
		return boost::apply_visitor(binary_visitor<const T1, const T2>(lhs, rhs), variant_binary);
	}

	VariantBinary variant_binary;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary> struct binary_adaptor<Binary> make_binary_adaptor(const Binary &binary) {
	return binary_adaptor<Binary>(binary);
}

} // namespace util

#endif /* BINARY_ADAPTOR_HPP_ */
