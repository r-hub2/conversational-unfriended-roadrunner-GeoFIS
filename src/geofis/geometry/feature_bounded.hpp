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
#ifndef H8B6165AC_994C_42F8_93B4_670248349757
#define H8B6165AC_994C_42F8_93B4_670248349757

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_2_algorithms.h>

namespace geofis {

template <class Kernel> class feature_bounded {

	typedef CGAL::Polygon_2<Kernel> polygon_type;

	polygon_type border;

public:
	feature_bounded(const polygon_type &border) : border(border) {}

	template <class Feature> bool operator()(const Feature &feature) const {
		return CGAL::bounded_side_2(border.vertices_begin(), border.vertices_end(), feature.get_geometry()) != CGAL::ON_UNBOUNDED_SIDE;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> inline feature_bounded<Kernel> make_feature_bounded(const CGAL::Polygon_2<Kernel> &border) {
	return feature_bounded<Kernel>(border);
}

} // namespace geofis

#endif // H8B6165AC_994C_42F8_93B4_670248349757 
