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
#ifndef FOR_ALL_ITERATOR_HPP_
#define FOR_ALL_ITERATOR_HPP_

#include <boost/iterator/transform_iterator.hpp>

namespace boost { namespace detail {

template <class UnaryFunc> struct function_object_result {

	typedef typename UnaryFunc::result_type type;
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class Return, class Argument> struct function_object_result<Return(*)(Argument)> {

	typedef Return type;
};
#endif

template <class T, class DefaultNullaryFn> struct ia_dflt_help : mpl::eval_if<is_same<T, use_default>, DefaultNullaryFn, mpl::identity<T> > {};

}} // namespace boost::detail

namespace util {

template <class Binary, class Iterator1, class Iterator2, class Reference = boost::use_default, class Value = boost::use_default> class transform_all_iterator;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary, class Iterator1, class Iterator2, class Reference, class Value> struct transform_all_iterator_traits {

    typedef typename boost::detail::ia_dflt_help<Reference, boost::detail::function_object_result<Binary> >::type reference;
    typedef typename boost::detail::ia_dflt_help<Value, boost::remove_reference<reference> >::type value_type;

	typedef boost::iterator_facade<transform_all_iterator<Binary, Iterator1, Iterator2, Reference, Value>, value_type, std::forward_iterator_tag, reference> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary, class Iterator1, class Iterator2, class Reference, class Value> class transform_all_iterator : public transform_all_iterator_traits<Binary, Iterator1, Iterator2, Reference, Value>::base_type {

	typedef typename transform_all_iterator_traits<Binary, Iterator1, Iterator2, Reference, Value>::base_type base_type;

    friend class boost::iterator_core_access;

public:
	typedef typename base_type::reference reference;

	transform_all_iterator() {}
    transform_all_iterator(Iterator1 iterator1, Iterator2 begin2, Iterator2 end2, const Binary &binary) : iterator1(iterator1), begin2(begin2), iterator2(begin2), end2(end2), binary(binary) {}

private:
    reference dereference() const { return binary(*iterator1, *iterator2); }

    void increment() {
    	++iterator2;
    	if(iterator2 == end2) {
    		++iterator1;
    		iterator2 = begin2;
    	}
    }

    bool equal(const transform_all_iterator &other) const {
    	return (iterator1 == other.iterator1) && (iterator2 == other.iterator2);
    }

    Iterator1 iterator1;
    Iterator2 begin2;
    Iterator2 iterator2;
    Iterator2 end2;
    Binary binary;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Binary, class Iterator1, class Iterator2> inline transform_all_iterator<Binary, Iterator1, Iterator2> make_transform_all_iterator(Iterator1 iterator1, Iterator2 begin2, Iterator2 end2, const Binary &binary) {
	return transform_all_iterator<Binary, Iterator1, Iterator2>(iterator1, begin2, end2, binary);
}

} // namespace util

#endif /* FOR_ALL_ITERATOR_HPP_ */
