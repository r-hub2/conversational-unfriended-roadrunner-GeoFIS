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
#ifndef GEOMETRY_ITERATOR_HPP_
#define GEOMETRY_ITERATOR_HPP_

#include <boost/iterator/transform_iterator.hpp>

namespace geofis {

template <class Feature> struct geometry_function {

	typedef const typename Feature::geometry_type &result_type;

	inline result_type operator() (Feature &feature) const { return feature.get_geometry(); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureIterator> struct geometry_iterator_traits {

	typedef typename std::iterator_traits<FeatureIterator>::value_type feature_type;
	typedef boost::transform_iterator<geometry_function<const feature_type>, FeatureIterator> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureIterator> class geometry_iterator : public geometry_iterator_traits<FeatureIterator>::base_type {

	typedef typename geometry_iterator_traits<FeatureIterator>::base_type base_type;
	typedef typename geometry_iterator_traits<FeatureIterator>::feature_type feature_type;

public:
	geometry_iterator(const FeatureIterator &feature_iterator) : base_type(feature_iterator, geometry_function<const feature_type>()) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeatureIterator> geometry_iterator<FeatureIterator> make_geometry_iterator(const FeatureIterator &feature_iterator) {
	return geometry_iterator<FeatureIterator>(feature_iterator);
}

} // namespace geofis

#endif /* GEOMETRY_ITERATOR_HPP_ */
