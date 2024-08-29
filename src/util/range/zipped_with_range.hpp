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
#ifndef H932FF024_DCF8_428F_8455_3E34B933B809
#define H932FF024_DCF8_428F_8455_3E34B933B809

#include <boost/range/combine.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace util {

template <class Function> struct zip_with_adapter {

	typedef typename Function::result_type result_type;

	Function function;

	zip_with_adapter(Function function) : function(function) {}

	template <class T0, class T1> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::null_type> > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>());
	}

	template <class T0, class T1, class T2> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::null_type> > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>());
	}

	template <class T0, class T1, class T2, class T3> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::null_type> > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>());
	}

	template <class T0, class T1, class T2, class T3, class T4> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::null_type> > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>());
	}

	template <class T0, class T1, class T2, class T3, class T4, class T5> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::cons<T5, boost::tuples::null_type> > > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>(), tuple.template get<5>());
	}

	template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::cons<T5, boost::tuples::cons<T6, boost::tuples::null_type> > > > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>(), tuple.template get<5>(), tuple.template get<6>());
	}

	template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::cons<T5, boost::tuples::cons<T6, boost::tuples::cons<T7, boost::tuples::null_type> > > > > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>(), tuple.template get<5>(), tuple.template get<6>(), tuple.template get<7>());
	}

	template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::cons<T5, boost::tuples::cons<T6, boost::tuples::cons<T7, boost::tuples::cons<T8, boost::tuples::null_type> > > > > > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>(), tuple.template get<5>(), tuple.template get<6>(), tuple.template get<7>(), tuple.template get<8>());
	}

	template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> result_type operator()(const boost::tuples::cons<T0, boost::tuples::cons<T1, boost::tuples::cons<T2, boost::tuples::cons<T3, boost::tuples::cons<T4, boost::tuples::cons<T5, boost::tuples::cons<T6, boost::tuples::cons<T7, boost::tuples::cons<T8, boost::tuples::cons<T9, boost::tuples::null_type> > > > > > > > > > &tuple) const {
		return function(tuple.template get<0>(), tuple.template get<1>(), tuple.template get<2>(), tuple.template get<3>(), tuple.template get<4>(), tuple.template get<5>(), tuple.template get<6>(), tuple.template get<7>(), tuple.template get<8>(), tuple.template get<9>());
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Function, class... Ranges> auto make_zipped_with(Function function, Ranges&&... ranges) {
	return boost::combine(ranges...) | boost::adaptors::transformed(zip_with_adapter<Function>(function));
}

} // namespace util

#endif // H932FF024_DCF8_428F_8455_3E34B933B809 
