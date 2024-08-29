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
#ifndef AGGREGATION_ADAPTOR_HPP_
#define AGGREGATION_ADAPTOR_HPP_

#include <boost/range.hpp>
#include <boost/variant/is_variant.hpp>

namespace geofis {

template <class Aggregation, class IsVariant = typename boost::is_variant<Aggregation>::type> struct aggregation_adaptor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Aggregation> struct aggregation_adaptor<Aggregation, boost::false_type> {

	aggregation_adaptor() {}
	aggregation_adaptor(const Aggregation &aggregation) : aggregation(aggregation) {}
	template <BOOST_VARIANT_ENUM_PARAMS(class T)> aggregation_adaptor(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &aggregation) : aggregation(boost::get<Aggregation>(aggregation)) {}

	template <class ZonePairRange, class OutputIterator> void operator() (ZonePairRange &zone_pairs, OutputIterator output) const {
		aggregation(zone_pairs, output);
	}

	Aggregation aggregation;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class VariantAggregation> struct aggregation_adaptor<VariantAggregation, boost::true_type> {

	aggregation_adaptor() {}
	aggregation_adaptor(const VariantAggregation &variant_aggregation) : variant_aggregation(variant_aggregation) {}

	template <class ZonePairRange, class OutputIterator> void operator() (ZonePairRange &zone_pairs, OutputIterator output) const {
		boost::apply_visitor(aggregation_visitor<ZonePairRange, OutputIterator>(zone_pairs, output), variant_aggregation);
	}

	template <class ZonePairRange, class OutputIterator> struct aggregation_visitor : public boost::static_visitor<> {

		aggregation_visitor(ZonePairRange &zone_pairs, OutputIterator output) : zone_pairs(zone_pairs), output(output) {}

	    template <class Aggregation> void operator() (const Aggregation &aggregation) const {
	    	aggregation(zone_pairs, output);
	    }

	    ZonePairRange &zone_pairs;
	    OutputIterator output;
	};

	VariantAggregation variant_aggregation;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Aggregation> aggregation_adaptor<Aggregation> make_aggregation_adaptor(const Aggregation &aggregation) {
	return aggregation_adaptor<Aggregation>(aggregation);
}

} // namespace geofis

#endif /* AGGREGATION_ADAPTOR_HPP_ */
