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
#ifndef ZONE_PAIR_DISTANCE_HPP_
#define ZONE_PAIR_DISTANCE_HPP_

#include <util/dont_care.hpp>
#include <util/functional/maximum.hpp>
#include <util/functional/minimum.hpp>
#include <util/functional/mean.hpp>
#include <boost/variant.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/mpl/transform.hpp>
#include <geofis/algorithm/zoning/pair/accumulator_zone_pair_distance.hpp>

namespace geofis {

/*
@startuml

skinparam {
	linetype ortho
}

class accumulator_zone_pair_distance<Feature>

interface zone_pair_distance<ZoneDistance>

class minimum_zone_pair_distance {
	+ <<constructor>> minimum_zone_pair_distance(feature_distances: FeatureDistanceRange)
}

class maximum_zone_pair_distance {
	+ <<constructor>> maximum_zone_pair_distance(feature_distances: FeatureDistanceRange)
}

class mean_zone_pair_distance {
	+ <<constructor>> mean_zone_pair_distance(feature_distances: FeatureDistanceRange)
	+ get_distance(): double
}

class variant_zone_pair_distance {
	+ <<constructor>> mean_zone_pair_distance(feature_distances: FeatureDistanceRange)
	+ get_distance(): double
}

zone_pair_distance <|.down. minimum_zone_pair_distance: <<bind>> \n <ZoneDistance -> util::minimum<double>>
zone_pair_distance <|.down. maximum_zone_pair_distance: <<bind>> \n <ZoneDistance -> util::maximum<double>>
zone_pair_distance <|.down. mean_zone_pair_distance: <<bind>> \n <ZoneDistance -> util::mean<double>>
zone_pair_distance <|.down. variant_zone_pair_distance: <<bind>> \n <ZoneDistance -> boost::variant<T...>>

accumulator_zone_pair_distance <|-- minimum_zone_pair_distance: <<bind>> \n <Feature -> boost::accumulators::tag::min>
accumulator_zone_pair_distance <|-- maximum_zone_pair_distance: <<bind>> \n <Feature -> boost::accumulators::tag::max>

hide accumulator_zone_pair_distance members
hide zone_pair_distance members

@enduml
*/

//! \brief Class dedicated to compute distance between 2 zones in a zone_pair
// TODO rename zone_pair_distance to zone_distance_computer
template <class ZoneDistance> class zone_pair_distance;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class ZoneDistance, class FeatureDistanceRange> inline zone_pair_distance<ZoneDistance> make_zone_pair_distance(const ZoneDistance &zone_distance, const FeatureDistanceRange &feature_distances) {
	return zone_pair_distance<ZoneDistance>(zone_distance, feature_distances);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! \brief Specialization of zone_pair_distance for minimum zone distance
template <> class zone_pair_distance<util::minimum<double> > : public accumulator_zone_pair_distance<boost::accumulators::tag::min> {

	typedef accumulator_zone_pair_distance<boost::accumulators::tag::min> base_type;

public:
	template <class FeatureDistanceRange> zone_pair_distance(util::dont_care, const FeatureDistanceRange &feature_distances) : base_type(feature_distances) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! \brief Specialization of zone_pair_distance for maximum zone distance
template <> class zone_pair_distance<util::maximum<double> > : public accumulator_zone_pair_distance<boost::accumulators::tag::max>  {

	typedef accumulator_zone_pair_distance<boost::accumulators::tag::max> base_type;

public:
	template <class FeatureDistanceRange> zone_pair_distance(util::dont_care, const FeatureDistanceRange &feature_distances) : base_type(feature_distances) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! \brief Specialization of zone_pair_distance for mean zone distance
template <> class zone_pair_distance<util::mean<double> > {

	typedef boost::accumulators::accumulator_set<double, boost::accumulators::features<boost::accumulators::tag::sum, boost::accumulators::tag::count> > accumulator_type;

public:
	template <class FeatureDistanceRange> zone_pair_distance(util::dont_care, const FeatureDistanceRange &feature_distances) : sum(0.), count(0) {
		update_feature_distances(feature_distances);
	}

	double get_distance() const {
#ifdef __MINGW32__
		// convert std::size_t to int otherwise ununderstandable warning "maybe-uninitialized"
		return get_sum() / boost::numeric_cast<int>(get_count());
#else
		return get_sum() / get_count();
#endif
	}

	void update(const zone_pair_distance &other) {
		sum += other.get_sum();
		count += other.get_count();
	}

	template <class FeatureDistanceRange> void update_feature_distances(const FeatureDistanceRange &feature_distances) {
		boost::for_each(feature_distances, boost::bind<void>(boost::ref(accumulator), boost::placeholders::_1));
	}

private:
	accumulator_type accumulator;
	double sum;
	std::size_t count;

	double get_sum() const {
		return sum + boost::accumulators::sum(accumulator);
	}

	std::size_t get_count() const {
		return count + boost::accumulators::count(accumulator);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! \brief Specialization of zone_pair_distance for variant zone distance
template <class... Types> class zone_pair_distance<boost::variant<Types...> > {

	typedef boost::variant<Types...> zone_distance_type;
	typedef typename zone_distance_type::types zone_distance_types;
	typedef typename boost::mpl::transform<zone_distance_types, zone_pair_distance<boost::mpl::_1> >::type zone_pair_distance_sequence_type;
	typedef typename boost::make_variant_over<zone_pair_distance_sequence_type>::type variant_zone_pair_distance_type;

public:
	template <class FeatureDistanceRange> zone_pair_distance(const zone_distance_type &zone_distance, const FeatureDistanceRange &feature_distances) : variant_zone_pair_distance(boost::apply_visitor(make_variant_zone_pair_distance_initializer(feature_distances), zone_distance)) {}

	double get_distance() const {
		return boost::apply_visitor(distance_getter(), variant_zone_pair_distance);
	}

	void update(const zone_pair_distance &other) {
		boost::apply_visitor(updater(), variant_zone_pair_distance, other.variant_zone_pair_distance);
	}

	template <class FeatureDistanceRange> void update_feature_distances(const FeatureDistanceRange &feature_distances) {
		boost::apply_visitor(make_feature_distances_updater(feature_distances), variant_zone_pair_distance);
	}

private:
	variant_zone_pair_distance_type variant_zone_pair_distance;

	template <class FeatureDistanceRange> struct variant_zone_pair_distance_initializer : public boost::static_visitor<variant_zone_pair_distance_type> {

		variant_zone_pair_distance_initializer(const FeatureDistanceRange &feature_distances) : feature_distances(feature_distances) {}

	    template <class ZoneDistance> variant_zone_pair_distance_type operator()(const ZoneDistance &zone_distance) const {
	    	return variant_zone_pair_distance_type(make_zone_pair_distance(zone_distance, feature_distances));
	    }

	    const FeatureDistanceRange &feature_distances;
	};

	template <class FeatureDistanceRange> static inline variant_zone_pair_distance_initializer<FeatureDistanceRange> make_variant_zone_pair_distance_initializer(const FeatureDistanceRange &feature_distances) {
		return variant_zone_pair_distance_initializer<FeatureDistanceRange>(feature_distances);
	}

	struct distance_getter : public boost::static_visitor<double> {

		template <class ZonePairDistance> double operator()(const ZonePairDistance &zone_pair_distance) const {
			return zone_pair_distance.get_distance();
		}
	};

	struct updater : public boost::static_visitor<> {

		template <class ZonePairDistance> void operator()(ZonePairDistance &zone_pair_distance, const ZonePairDistance &other) const {
			zone_pair_distance.update(other);
		}

		template <class T, class U> void operator()(const T &, const U &) const {
		}
	};

	template <class FeatureDistanceRange> struct feature_distances_updater : public boost::static_visitor<> {

		feature_distances_updater(const FeatureDistanceRange &feature_distances) : feature_distances(feature_distances) {}

	    template <class ZonePairDistance> void operator()(ZonePairDistance &zone_pair_distance) const {
	    	zone_pair_distance.update_feature_distances(feature_distances);
	    }

	    const FeatureDistanceRange &feature_distances;
	};

	template <class FeatureDistanceRange> static inline feature_distances_updater<FeatureDistanceRange> make_feature_distances_updater(const FeatureDistanceRange &feature_distances) {
		return feature_distances_updater<FeatureDistanceRange>(feature_distances);
	}
};

} // namespace geofis

#endif /* ZONE_PAIR_DISTANCE_HPP_ */
