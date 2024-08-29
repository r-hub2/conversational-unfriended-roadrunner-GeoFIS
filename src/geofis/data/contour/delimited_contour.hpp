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
#ifndef H14BA5814_4331_462C_9CAB_0E17BFBABB35
#define H14BA5814_4331_462C_9CAB_0E17BFBABB35

#include <stdexcept>
#include <CGAL/Polygon_2.h>
#include <CGAL/Point_2.h>
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <util/data/data_loader.hpp>
#include <util/data/file/delimited_column_loader.hpp>
#include <util/data/coupling_loader.hpp>
#include <geofis/geometry/point_2_maker.hpp>
#include <geofis/geometry/polygon.hpp>

namespace geofis {

template <class Kernel> CGAL::Polygon_2<Kernel> get_delimited_contour(const boost::filesystem::path &path) {

	typedef CGAL::Point_2<Kernel> point_type;
	typedef CGAL::Polygon_2<Kernel> polygon_type;
	typedef point_2_maker<Kernel> point_maker_type;
	typedef typename util::delimited_column_loader_traits<double>::type delimited_column_loader_type;
	typedef util::coupling_loader<delimited_column_loader_type, delimited_column_loader_type, point_maker_type> point_loader_type;
	typedef util::data_loader<point_loader_type> data_loader_type;

	delimited_column_loader_type x_loader = util::make_delimited_column_loader<double>(path, 0);
	delimited_column_loader_type y_loader = util::make_delimited_column_loader<double>(path, 1);
	point_loader_type point_loader = util::make_coupling_loader(x_loader, y_loader, point_maker_type());
	data_loader_type data(point_loader);
	polygon_type contour(boost::begin(data), boost::end(data));
	if(!is_valid_polygon(contour))
		throw std::runtime_error(boost::str(boost::format("%1% is not a valid csv contour") % path));
	return contour;
}

} // namespace geofis

#endif // H14BA5814_4331_462C_9CAB_0E17BFBABB35 
