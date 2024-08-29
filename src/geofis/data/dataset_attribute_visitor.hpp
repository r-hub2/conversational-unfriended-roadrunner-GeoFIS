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
#ifndef DATASET_ATTRIBUTE_VISITOR_HPP_
#define DATASET_ATTRIBUTE_VISITOR_HPP_

/* Fix for jlong on 64-bit x86 Solaris */
#if defined(__x86_64)
# ifdef _LP64
#   undef _LP64
# endif
#endif

#include <jni.h>
#include <string>
#include <util/data/data.hpp>
#include <boost/variant/static_visitor.hpp>
#include <CGAL/Point_2.h>

namespace geofis {

class dataset_attribute_visitor : public boost::static_visitor<void> {

public:
	dataset_attribute_visitor() : jenv(NULL), object(NULL) {}
	dataset_attribute_visitor(JNIEnv *jenv, jobject object) : jenv(jenv), object(object) {}

	template <class T> void operator()(const util::data<T> &data) { invoke(jni_class_name<T>::get(), (jlong)&data, jni_method_signature<T>::get()); }

private:
	JNIEnv *jenv;
	jobject object;

	template <class T> struct jni_class_name {

		static const char *get();
	};

	template <class T> struct jni_method_signature {

		static const char *get();
	};

	void invoke(const char *class_name, jlong data, const char *method_signature);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <> const char *dataset_attribute_visitor::jni_class_name<double>::get();
template <> const char *dataset_attribute_visitor::jni_class_name<int32_t>::get();
template <> const char *dataset_attribute_visitor::jni_class_name<std::string>::get();

template <class Kernel> struct dataset_attribute_visitor::jni_class_name<CGAL::Point_2<Kernel> > {

	static const char *get() { return "org/geofis/data/DataPoint2"; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <> const char *dataset_attribute_visitor::jni_method_signature<double>::get();
template <> const char *dataset_attribute_visitor::jni_method_signature<int32_t>::get();
template <> const char *dataset_attribute_visitor::jni_method_signature<std::string>::get();

template <class Kernel> struct dataset_attribute_visitor::jni_method_signature<CGAL::Point_2<Kernel> > {

	static const char *get() { return "(Lorg/geofis/data/DataPoint2;)V"; }
};

} // namespace geofis

#endif /* DATASET_ATTRIBUTE_VISITOR_HPP_ */
