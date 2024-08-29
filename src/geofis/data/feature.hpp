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
#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <cstddef>
#include <functional>
#include <boost/variant.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <util/assert.hpp>
#include <util/range/assign.hpp>
#include <geofis/data/default_attribute_type.hpp>
#include <geofis/data/attribute_equality.hpp>
#include <geofis/data/is_monodimensional_feature.hpp>

/*
@startuml

interface feature<Id, Geometry, Attribute, IsMonoAttribute>

hide feature members

class multidimensional_feature<Id, Geometry, AttributeRange> {
- id: Id
- geometry: Geometry
- attributes: Attribute [1..*]
- normalized_attributes: Attribute [0..*]
}

feature <|.down. multidimensional_feature: <<bind>> \n <IsMonoAttribute -> false>

class monodimensional_feature<Id, Geometry, Attribute> {
- id: Id
- geometry: Geometry
- attribute: Attribute
- normalized_attribute: Attribute
}

feature <|.down. monodimensional_feature: <<bind>> \n <IsMonoAttribute -> true>

@enduml
*/

namespace geofis {

template <class Id, class Geometry, class Attribute, class IsMonoAttribute = typename boost::mpl::contains<default_attribute_type, Attribute>::type> class feature;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature for mono attribute

template <class Id, class Geometry, class Attribute> class feature<Id, Geometry, Attribute, boost::mpl::true_> {

public:
	typedef Id id_type;
	typedef Geometry geometry_type;
	typedef Attribute attribute_type;

	feature(Id id, const Geometry &geometry, const Attribute &attribute) : id(id), geometry(geometry), attribute(attribute) {}

	Id get_id() const { return id; }

	Geometry get_geometry() const { return geometry; }

	const Attribute &get_attribute() const { return attribute; }

	const Attribute &get_normalized_attribute() const { return normalized_attribute; }

	void set_normalized_attribute(const Attribute &normalized_attribute) { this->normalized_attribute = normalized_attribute; }

	friend bool operator == (const feature &lhs, const feature &rhs) { return (lhs.id == rhs.id) && (lhs.geometry == rhs.geometry) && attribute_equals(lhs.attribute, rhs.attribute); }

private:
	Id id;
	Geometry geometry;
	Attribute attribute;
	Attribute normalized_attribute;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of is_monodimensional_feature for monodimensional feature

template <class Id, class Geometry, class Attribute> struct is_monodimensional_feature<feature<Id, Geometry, Attribute, boost::mpl::true_> > : boost::true_type {};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// specialization of feature for multi attribute

template <class Id, class Geometry, class AttributeRange> class feature<Id, Geometry, AttributeRange, boost::mpl::false_> {

public:
	typedef Id id_type;
	typedef Geometry geometry_type;
	typedef AttributeRange attribute_range_type;
	typedef typename boost::range_value<attribute_range_type>::type attribute_type;

	typedef typename boost::range_iterator<const AttributeRange>::type const_attribute_iterator_type;
	typedef typename boost::sub_range<const AttributeRange>::type const_attribute_range_type;

	feature(Id id, const Geometry &geometry, const AttributeRange &attributes) : id(id), geometry(geometry), attributes(attributes) {}
	template <class OtherAttributeRange> feature(Id id, const Geometry &geometry, const OtherAttributeRange &other_attributes) : id(id), geometry(geometry), attributes(boost::begin(other_attributes), boost::end(other_attributes)) {}
	template <class OtherAttributeRange> feature(const feature<Id, Geometry, OtherAttributeRange> &other_feature) : id(other_feature.get_id()), geometry(other_feature.get_geometry()), attributes(other_feature.get_attribute_begin(), other_feature.get_attribute_end()) {}

	Id get_id() const { return id; }

	Geometry get_geometry() const { return geometry; }

	const_attribute_iterator_type get_attribute_begin() const { return boost::begin(attributes); }
	const_attribute_iterator_type get_attribute_end() const { return boost::end(attributes); }

	const_attribute_range_type get_attribute_range() const { return attributes; }

	const_attribute_range_type get_normalized_attribute_range() const { return normalized_attributes; }

	template <class OtherAttributeRange> void set_normalized_attribute_range(const OtherAttributeRange &normalized_attributes) {

		BOOST_MPL_ASSERT((boost::is_same<typename boost::range_value<AttributeRange>::type, typename boost::range_value<OtherAttributeRange>::type>));

		UTIL_REQUIRE(boost::size(normalized_attributes) == get_attribute_size());
		util::assign(this->normalized_attributes, normalized_attributes);
	}

	size_t get_attribute_size() const { return boost::size(attributes); }
	size_t get_normalized_attribute_size() const { return boost::size(normalized_attributes); }

    template <class Visitor> Visitor &apply_attribute_visitor(Visitor &visitor) const;

	friend bool operator == (const feature &lhs, const feature &rhs) { return (lhs.id == rhs.id) && (lhs.geometry == rhs.geometry) && boost::equal(lhs.attributes, rhs.attributes, attribute_equality()); }
	friend bool operator != (const feature &lhs, const feature &rhs) { return !(lhs == rhs); }

private:
    Id id;
	Geometry geometry;
	AttributeRange attributes;
	AttributeRange normalized_attributes;

	template <class Visitor> struct internal_attribute_visitor : public boost::static_visitor<void> {

		internal_attribute_visitor(Visitor &visitor) : visitor(visitor) {}

		template <class T> void operator() (const T &value) { visitor(value); }

		template <BOOST_VARIANT_ENUM_PARAMS(class T)> void operator() (const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &variant) { boost::apply_visitor(*this, variant); }

		Visitor &visitor;
	};

	template <class Visitor> static inline internal_attribute_visitor<Visitor> make_visitor(Visitor &visitor) {
		return internal_attribute_visitor<Visitor>(visitor);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Id, class Geometry, class AttributeRange> template <class Visitor> Visitor &feature<Id, Geometry, AttributeRange, boost::mpl::false_>::apply_attribute_visitor(Visitor &visitor) const {
	visitor(geometry);
	return boost::for_each(attributes, make_visitor(visitor)).visitor;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Visitor, class Feature> inline Visitor &apply_feature_attribute_visitor(Visitor& visitor, Feature& feature) {
    return feature.apply_attribute_visitor(visitor);
}

template <class Visitor, class Feature> inline const Visitor &apply_feature_attribute_visitor(const Visitor& visitor, Feature& feature) {
    return feature.apply_attribute_visitor(visitor);
}

} // namespace geofis

#endif /* FEATURE_HPP_ */
