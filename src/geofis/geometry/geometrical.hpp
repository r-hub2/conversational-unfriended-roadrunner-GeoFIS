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
#ifndef GEOMETRICAL_HPP_
#define GEOMETRICAL_HPP_

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace geofis {

template <class Geometrical> struct geometrical_traits {

	typedef typename Geometrical::geometry_type geometry_type;
	//typedef typename boost::mpl::if_<boost::is_const<Geometrical>, const geometry_type &, geometry_type &>::type geometry_reference_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometrical> struct geometry_getter {

	typedef typename geometrical_traits<Geometrical>::geometry_type result_type;
	//typedef typename geometrical_traits<Geometrical>::geometry_reference_type result_type;

	result_type operator()(Geometrical &geometrical) const {
		return geometrical.get_geometry();
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class GeometricalRange> struct geometrical_range_traits {

	typedef typename boost::range_value<GeometricalRange>::type geometrical_type;
	//typedef typename boost::mpl::if_<boost::is_const<GeometricalRange>, geometry_getter<const geometrical_type>, geometry_getter<geometrical_type> >::type geometry_getter_type;
	//typedef boost::transformed_range<geometry_getter_type, GeometricalRange> geometry_range_type;
	typedef geometry_getter<const geometrical_type> geometry_getter_type;
	typedef boost::transformed_range<geometry_getter_type, GeometricalRange> geometry_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*template <class GeometricalRange> inline typename geometrical_range_traits<GeometricalRange>::geometry_range_type make_geometry_range(GeometricalRange &geometricals) {

	typedef typename geometrical_range_traits<GeometricalRange>::geometrical_type geometrical_type;

	return boost::adaptors::transform(geometricals, geometry_getter<geometrical_type>());
}*/

template <class GeometricalRange> inline typename geometrical_range_traits<const GeometricalRange>::geometry_range_type make_geometry_range(const GeometricalRange &geometricals) {

	//typedef typename geometrical_range_traits<const GeometricalRange>::geometrical_type geometrical_type;
	typedef typename geometrical_range_traits<const GeometricalRange>::geometry_getter_type geometry_getter_type;

	return boost::adaptors::transform(geometricals, geometry_getter_type());
}

} // namespace geofis

#endif /* GEOMETRICAL_HPP_ */
