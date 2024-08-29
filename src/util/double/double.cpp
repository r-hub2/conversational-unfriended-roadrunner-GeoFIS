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
#include <util/double.h>

#include <util/double/boost_double_comparison.h>
//#include <util/double/std_double_comparison.h>

namespace std {

equal_to<float>::equal_to() : tolerance(util::detail::tolerance<float>()) {}
bool equal_to<float>::operator() (const float &x, const float &y) const { return util::detail::is_equal(x, y, tolerance); }

equal_to<double>::equal_to() : tolerance(util::detail::tolerance<double>()) {}
bool equal_to<double>::operator() (const double &x, const double &y) const { return util::detail::is_equal(x, y, tolerance); }

equal_to<long double>::equal_to() : tolerance(util::detail::tolerance<long double>()) {}
bool equal_to<long double>::operator() (const long double &x, const long double &y) const { return util::detail::is_equal(x, y, tolerance); }

not_equal_to<float>::not_equal_to() : tolerance(util::detail::tolerance<float>()) {}
bool not_equal_to<float>::operator() (const float &x, const float &y) const { return !util::detail::is_equal(x, y, tolerance); }

not_equal_to<double>::not_equal_to() : tolerance(util::detail::tolerance<double>()) {}
bool not_equal_to<double>::operator() (const double &x, const double &y) const { return !util::detail::is_equal(x, y, tolerance); }

not_equal_to<long double>::not_equal_to() : tolerance(util::detail::tolerance<long double>()) {}
bool not_equal_to<long double>::operator() (const long double &x, const long double &y) const { return !util::detail::is_equal(x, y, tolerance); }

less<float>::less() : tolerance(util::detail::tolerance<float>()) {}
bool less<float>::operator() (const float &x, const float &y) const { return util::detail::is_less(x, y, tolerance); }

less<double>::less() : tolerance(util::detail::tolerance<double>()) {}
bool less<double>::operator() (const double &x, const double &y) const { return util::detail::is_less(x, y, tolerance); }

less<long double>::less() : tolerance(util::detail::tolerance<long double>()) {}
bool less<long double>::operator() (const long double &x, const long double &y) const { return util::detail::is_less(x, y, tolerance); }

greater<float>::greater() : tolerance(util::detail::tolerance<float>()) {}
bool greater<float>::operator() (const float &x, const float &y) const { return util::detail::is_greater(x, y, tolerance); }

greater<double>::greater() : tolerance(util::detail::tolerance<double>()) {}
bool greater<double>::operator() (const double &x, const double &y) const { return util::detail::is_greater(x, y, tolerance); }

greater<long double>::greater() : tolerance(util::detail::tolerance<long double>()) {}
bool greater<long double>::operator() (const long double &x, const long double &y) const { return util::detail::is_greater(x, y, tolerance); }

less_equal<float>::less_equal() : tolerance(util::detail::tolerance<float>()) {}
bool less_equal<float>::operator() (const float &x, const float &y) const { return util::detail::is_less_equal(x, y, tolerance); }

less_equal<double>::less_equal() : tolerance(util::detail::tolerance<double>()) {}
bool less_equal<double>::operator() (const double &x, const double &y) const { return util::detail::is_less_equal(x, y, tolerance); }

less_equal<long double>::less_equal() : tolerance(util::detail::tolerance<long double>()) {}
bool less_equal<long double>::operator() (const long double &x, const long double &y) const { return util::detail::is_less_equal(x, y, tolerance); }

greater_equal<float>::greater_equal() : tolerance(util::detail::tolerance<float>()) {}
bool greater_equal<float>::operator() (const float &x, const float &y) const { return util::detail::is_greater_equal(x, y, tolerance); }

greater_equal<double>::greater_equal() : tolerance(util::detail::tolerance<double>()) {}
bool greater_equal<double>::operator() (const double &x, const double &y) const { return util::detail::is_greater_equal(x, y, tolerance); }

greater_equal<long double>::greater_equal() : tolerance(util::detail::tolerance<long double>()) {}
bool greater_equal<long double>::operator() (const long double &x, const long double &y) const { return util::detail::is_greater_equal(x, y, tolerance); }

} // namespace std
