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
#ifndef FEATURE_ATTRIBUTE_VISITOR_HPP_
#define FEATURE_ATTRIBUTE_VISITOR_HPP_

/* Fix for jlong on 64-bit x86 Solaris */
#if defined(__x86_64)
# ifdef _LP64
#   undef _LP64
# endif
#endif

#include <jni.h>
#include <string>
#include <util/assert.hpp>
#include <boost/variant/static_visitor.hpp>
#include <CGAL/Point_2.h>

namespace geofis {

class feature_attribute_visitor : public boost::static_visitor<void> {

public:
	feature_attribute_visitor() : jenv(NULL), object(NULL) {}
	feature_attribute_visitor(JNIEnv *jenv, jobject object) : jenv(jenv), object(object) {}

	template <class T> void operator()(const T &attribute) { jni_caller<T>::invoke(jenv, object, attribute); }

private:
	JNIEnv *jenv;
	jobject object;

	template <class T> struct jni_method_signature {

		static const char *get();
	};

	template <class T> struct jni_attribute {

		static T get(const T &attribute) { return attribute; }
	};

	template <class T> struct jni_caller {

		static void invoke(JNIEnv *jenv, jobject object, const T &attribute) {
			feature_attribute_visitor::invoke(jenv, object, jni_method_signature<T>::get(), jni_attribute<T>::get(attribute));
		}
	};

	template <class T> static void invoke(JNIEnv *jenv, jobject object, const char *method_signature, T attribute) {
		UTIL_REQUIRE(jenv && object);
		jmethodID invoke_method = jenv->GetMethodID(jenv->GetObjectClass(object), "visit", method_signature);
		UTIL_ASSERT(invoke_method);
		jenv->CallVoidMethod(object, invoke_method, attribute);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <> const char *feature_attribute_visitor::jni_method_signature<double>::get();
template <> const char *feature_attribute_visitor::jni_method_signature<int32_t>::get();
template <> const char *feature_attribute_visitor::jni_method_signature<std::string>::get();

template <class Kernel> struct feature_attribute_visitor::jni_method_signature<CGAL::Point_2<Kernel> > {

	static const char *get() { return "(Lorg/geofis/geometry/Point2;)V"; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <> struct feature_attribute_visitor::jni_attribute<std::string>;

template <class Kernel> struct feature_attribute_visitor::jni_attribute<CGAL::Point_2<Kernel> > {

	static jobject get(JNIEnv *jenv, const CGAL::Point_2<Kernel> &point) {
		UTIL_REQUIRE(jenv);
		jclass point_2_class = jenv->FindClass("org/geofis/geometry/Point2");
		UTIL_ASSERT(point_2_class);
		jmethodID point_2_constructor = jenv->GetMethodID(point_2_class, "<init>", "(JZ)V");
		UTIL_ASSERT(point_2_constructor);
		return jenv->NewObject(point_2_class, point_2_constructor, (jlong)&point, (jboolean)false);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Kernel> struct feature_attribute_visitor::jni_caller<CGAL::Point_2<Kernel> > {

	static void invoke(JNIEnv *jenv, jobject object, const CGAL::Point_2<Kernel> &point) {
		feature_attribute_visitor::invoke(jenv, object, jni_method_signature<CGAL::Point_2<Kernel> >::get(), jni_attribute<CGAL::Point_2<Kernel> >::get(jenv, point));
	}
};

} // namespace geofis

#endif /* FEATURE_ATTRIBUTE_VISITOR_HPP_ */
