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
#ifndef ACCUMULATOR_ZONE_PAIR_DISTANCE_HPP_
#define ACCUMULATOR_ZONE_PAIR_DISTANCE_HPP_

#include <boost/ref.hpp>
#include <boost/bind/bind.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace geofis {

/*
@startuml

'TODO rename to feature_distance_aggregator ?
class associative_zone_distance_computer<Feature> {
	+ <<constructor>> associative_zone_distance_computer(feature_distances: FeatureDistanceRange)
	+ get_distance(): double
	+ update(other: accumulator_zone_pair_distance)
	+ update_feature_distances(feature_distances: FeatureDistanceRange)
}
note right: This class aggregate **feature_distances** in **zone_distance** \nbased on **Feature** in boost::accumulators::accumulator_set. \nThe calculation of the **zone_distance** is only valid for associative **Feature**, \nlike boost::accumulators::tag::min or boost::accumulators::tag::max

@enduml
*/

// TODO rename accumulator_zone_pair_distance to associative_zone_distance_computer
template <class Feature> class accumulator_zone_pair_distance {

	typedef boost::accumulators::accumulator_set<double, boost::accumulators::features<Feature> > accumulator_type;

public:
	template <class FeatureDistanceRange> accumulator_zone_pair_distance(const FeatureDistanceRange &feature_distances) {
		update_feature_distances(feature_distances);
	}

	double get_distance() const {
		return boost::accumulators::extract_result<Feature>(accumulator);
	}

	void update(const accumulator_zone_pair_distance &other) {
		// this update operation is only valid for associative Feature
		accumulator(other.get_distance());
	}

	template <class FeatureDistanceRange> void update_feature_distances(const FeatureDistanceRange &feature_distances) {
		boost::for_each(feature_distances, boost::bind<void>(boost::ref(accumulator), boost::placeholders::_1));
	}

private:
	accumulator_type accumulator;
};

} // namespace geofis

#endif /* ACCUMULATOR_ZONE_PAIR_DISTANCE_HPP_ */
