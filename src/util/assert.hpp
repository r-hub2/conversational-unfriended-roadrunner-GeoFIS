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
#ifndef ASSERT_HPP_
#define ASSERT_HPP_

//#define NO_UTIL_ASSERT_TRHOW // to disable exception throwing on assertion failed, by default an exception is thrown when assertion is failed (in debug and in release mode)

#ifdef R_PACKAGE

#define UTIL_ASSERT(condition) ((void)0)
#define UTIL_ALLEGE(condition) ((void)0)

#else // NOT R_PACKAGE

#ifdef NDEBUG // RELEASE

#ifdef NO_UTIL_ASSERT_TRHOW // NO_THROW

#define UTIL_ASSERT(condition) ((void)0)
#define UTIL_ALLEGE(condition) ((void)(condition))

#else // TRHOW

namespace util {

	void release_assert(const char *message, char const *file, unsigned line);

} // namespace util

#define UTIL_ASSERT(condition) ((condition) ? (void)0 : util::release_assert(#condition, __FILE__, __LINE__))
#define UTIL_ALLEGE(condition) UTIL_ASSERT(condition)

#endif // END TRHOW

#else // DEBUG

namespace util {

void debug_assert(const char *message, char const *file, unsigned line);

} // namespace util

#define UTIL_ASSERT(condition) ((condition) ? (void)0 : util::debug_assert(#condition, __FILE__, __LINE__))
#define UTIL_ALLEGE(condition) UTIL_ASSERT(condition)

#endif // END DEBUG

#endif // R_PACKAGE

#define UTIL_REQUIRE(condition) UTIL_ASSERT(condition)
#define UTIL_ENSURE(condition) UTIL_ASSERT(condition)
#define UTIL_INVARIANT(condition) UTIL_ASSERT(condition)

#endif /* ASSERT_HPP_ */
