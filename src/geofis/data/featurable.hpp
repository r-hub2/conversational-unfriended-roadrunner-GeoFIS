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
#ifndef FEATURABLE_HPP_
#define FEATURABLE_HPP_

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace geofis {

template <class Featurable> struct featurable_traits {

	typedef typename Featurable::feature_type feature_type;
	typedef typename boost::mpl::if_<boost::is_const<Featurable>, const feature_type &, feature_type &>::type feature_reference_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Featurable> struct feature_getter {

	typedef typename featurable_traits<Featurable>::feature_reference_type result_type;

	result_type operator()(Featurable &featurable) const {
		return featurable.get_feature();
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeaturableRange> struct featurable_range_traits {

	typedef typename boost::range_value<FeaturableRange>::type featurable_type;
	typedef typename boost::mpl::if_<boost::is_const<FeaturableRange>, feature_getter<const featurable_type>, feature_getter<featurable_type> >::type feature_getter_type;
	typedef boost::transformed_range<feature_getter_type, FeaturableRange> feature_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FeaturableRange> inline typename featurable_range_traits<FeaturableRange>::feature_range_type make_feature_range(FeaturableRange &featurables) {

	typedef typename featurable_range_traits<FeaturableRange>::featurable_type featurable_type;

	return boost::adaptors::transform(featurables, feature_getter<featurable_type>());
}

template <class FeaturableRange> inline typename featurable_range_traits<const FeaturableRange>::feature_range_type make_feature_range(const FeaturableRange &featurables) {

	typedef typename featurable_range_traits<const FeaturableRange>::featurable_type featurable_type;

	return boost::adaptors::transform(featurables, feature_getter<const featurable_type>());
}

} // namespace geofis

#endif /* FEATURABLE_HPP_ */
