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
#ifndef FACE_TO_POLYGON_HPP_
#define FACE_TO_POLYGON_HPP_

#include <geofis/algorithm/zoning/triangulation/voronoi/bounded_face_to_polygon.hpp>
#include <geofis/algorithm/zoning/triangulation/voronoi/unbounded_face_to_polygon.hpp>

namespace geofis {

template <class Polygon> class face_to_polygon {

public:
	typedef Polygon result_type;

	face_to_polygon(const Polygon &boundary) : bounded(boundary), unbounded(boundary) {}

	template <class Face> result_type operator() (Face &face) const {
		return face.is_unbounded() ? unbounded(face) : bounded(face);
	}

private:
	bounded_face_to_polygon<Polygon> bounded;
	unbounded_face_to_polygon<Polygon> unbounded;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Polygon> inline face_to_polygon<Polygon> make_face_to_polygon(const Polygon &boundary) {
	return face_to_polygon<Polygon>(boundary);
}

} // namespace geofis

#endif /* FACE_TO_POLYGON_HPP_ */
