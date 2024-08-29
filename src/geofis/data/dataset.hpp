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
#ifndef GEOFIS_DATASET_HPP_
#define GEOFIS_DATASET_HPP_

#include <util/data/dataset.hpp>
#include <util/data/dataset_matrix_adaptor.hpp>
#include <geofis/data/default_attribute_type.hpp>
#include <geofis/data/feature_range.hpp>
#include <util/object/numbered_name_range.hpp>
#include <util/object/numbered_name_generator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/equal.hpp>

#include <util/type.hpp>
#include <util/data/dataset_util.hpp>

namespace geofis {

template <class Attribute, class IsSequence = typename boost::mpl::is_sequence<Attribute>::type> struct attribute_dataset_traits;

template <class Attribute> struct attribute_dataset_traits<Attribute, boost::mpl::true_> {

	typedef util::dataset<Attribute, util::variant_storage> type;
};

template <class Attribute> struct attribute_dataset_traits<Attribute, boost::mpl::false_> {

	typedef util::dataset<Attribute> type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class Attribute = default_attribute_type> class dataset {

	BOOST_MPL_ASSERT((boost::mpl::or_<boost::mpl::equal<default_attribute_type, Attribute>, boost::mpl::contains<default_attribute_type, Attribute> >));

	typedef util::data<Geometry> geometry_data_type;
	typedef typename geometry_data_type::const_iterator const_geometry_iterator;
	typedef typename geometry_data_type::unique_ptr geometry_data_ptr_type;
	//typedef util::dataset<Attribute, util::variant_storage> attribute_dataset_type;
	typedef typename attribute_dataset_traits<Attribute>::type attribute_dataset_type;

	typedef typename util::dataset_matrix_adaptor<Attribute> dataset_matrix_adaptor;

	typedef typename util::dataset_matrix_adaptor<Attribute>::const_range1 attribute_range1;
	typedef feature_range<geometry_data_type, attribute_range1> feature_range_type;

public:
	typedef boost::iterator_range<const_geometry_iterator> const_geometry_range_type;

	template <class GeometryLoader> dataset(const GeometryLoader &geometry_loader) : geometry(util::new_data(geometry_loader, "geometry")), attributes(), matrix_attributes_adaptor(attributes) {}
	template <class GeometryLoader, class AttributeLoaderRange> dataset(const GeometryLoader &geometry_loader, const AttributeLoaderRange &attribute_loader_range) : geometry(util::new_data(geometry_loader, "geometry")), attributes(attribute_loader_range/*, util::make_numbered_name_range(util::numbered_name_generator("attribute"))*/), matrix_attributes_adaptor(attributes) {}

	size_t data_size() const { return attributes.data_size(); }

    void set_name(const char *name) { this->name = name; }
    const char *get_name() const { return name.c_str(); }

    const util::data<Geometry> &get_geometry_data() const { return *geometry; }
    typename attribute_dataset_type::const_reference get_attribute_data(int index) const {
    	//std::cout << util::make_type(attributes[index]).name() << std::endl;
    	//std::cout << util::make_type(util::dataset_util::get_data<T>(attributes[index])).name() << std::endl;
    	//std::cout << util::Type<typename attribute_dataset_type::const_reference>::name() << std::endl;
    	return attributes[index];
    }

    const_geometry_range_type get_geometry_range() const {
    	return boost::make_iterator_range(*geometry);
    }

    feature_range_type get_feature_range() const { return make_feature_range(*geometry, matrix_attributes_adaptor.range1()); }

    template <class AttributeLoader> void push_back_attribute(const AttributeLoader &attribute_loader) { attributes.push_back(attribute_loader); }
    template <class AttributeLoader, class Name> void push_back_attribute(const AttributeLoader &attribute_loader, const Name &name) { attributes.push_back(attribute_loader, name); }

    template <class Visitor> Visitor &apply_attribute_visitor(Visitor &visitor) const;

private:
    geometry_data_ptr_type geometry;
	attribute_dataset_type attributes;
	dataset_matrix_adaptor matrix_attributes_adaptor;
	std::string name;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class Attribute> template <class Visitor> Visitor &dataset<Geometry, Attribute>::apply_attribute_visitor(Visitor &visitor) const {
	boost::apply_visitor(visitor, *geometry);
	return apply_data_visitor(visitor, attributes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Visitor, class Dataset> inline Visitor &apply_attribute_visitor(Visitor& visitor, Dataset& dataset) {
    return dataset.apply_attribute_visitor(visitor);
}

template <class Visitor, class Dataset> inline const Visitor &apply_attribute_visitor(const Visitor& visitor, Dataset& dataset) {
    return dataset.apply_attribute_visitor(visitor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class Attribute> struct geofis_dataset_traits {

	typedef dataset<Geometry, Attribute> dataset_type;
	typedef std::unique_ptr<dataset_type> dataset_ptr_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Geometry, class Attribute, class GeometryLoader, class AttributeLoaderRange> inline typename geofis_dataset_traits<Geometry, Attribute>::dataset_ptr_type new_geofis_dataset(const GeometryLoader &geometry_loader, const AttributeLoaderRange &attribute_loader_range) {

	typedef typename geofis_dataset_traits<Geometry, Attribute>::dataset_type dataset_type;
	typedef typename geofis_dataset_traits<Geometry, Attribute>::dataset_ptr_type dataset_ptr_type;

	return dataset_ptr_type(new dataset_type(geometry_loader, attribute_loader_range));
}

} // namespace geofis

#endif /* GEOFIS_DATASET_HPP_ */
