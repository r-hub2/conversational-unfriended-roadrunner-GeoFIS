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
#ifndef UTIL_DOUBLE_H_
#define UTIL_DOUBLE_H_

#include <functional>

namespace std {

#define FLOAT_SPECIALIZATION(class_to_specialize, type) \
\
template <> struct class_to_specialize<type> { \
\
	class_to_specialize(); \
	class_to_specialize(type tolerance) : tolerance(tolerance) {} \
\
	bool operator() (const type &x, const type &y) const; \
\
private: \
	type tolerance; \
};

FLOAT_SPECIALIZATION(equal_to, float)
FLOAT_SPECIALIZATION(equal_to, double)
FLOAT_SPECIALIZATION(equal_to, long double)

FLOAT_SPECIALIZATION(not_equal_to, float)
FLOAT_SPECIALIZATION(not_equal_to, double)
FLOAT_SPECIALIZATION(not_equal_to, long double)

FLOAT_SPECIALIZATION(less, float)
FLOAT_SPECIALIZATION(less, double)
FLOAT_SPECIALIZATION(less, long double)

FLOAT_SPECIALIZATION(greater, float)
FLOAT_SPECIALIZATION(greater, double)
FLOAT_SPECIALIZATION(greater, long double)

FLOAT_SPECIALIZATION(less_equal, float)
FLOAT_SPECIALIZATION(less_equal, double)
FLOAT_SPECIALIZATION(less_equal, long double)

FLOAT_SPECIALIZATION(greater_equal, float)
FLOAT_SPECIALIZATION(greater_equal, double)
FLOAT_SPECIALIZATION(greater_equal, long double)

} // namespace std

// temporary for compatibility
#define UTIL_DOUBLE_EQUAL(x, y) (std::equal_to<double>()((x), (y)))
#define UTIL_DOUBLE_LESS(x, y) (std::less<double>()((x), (y)))
#define UTIL_DOUBLE_GREATER(x, y) (std::greater<double>()((x), (y)))
#define UTIL_DOUBLE_LESS_EQUAL(x, y) (std::less_equal<double>()((x), (y)))
#define UTIL_DOUBLE_GREATER_EQUAL(x, y) (std::greater_equal<double>()((x), (y)))
#include <limits>
#define UTIL_DOUBLE_INFINITY std::numeric_limits<double>::infinity()

#endif /*UTIL_DOUBLE_H_*/
