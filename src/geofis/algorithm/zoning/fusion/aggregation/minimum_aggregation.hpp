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
#ifndef MINIMUM_AGGREGATION_HPP_
#define MINIMUM_AGGREGATION_HPP_

#include <list>
#include <boost/range.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance_less.hpp>
#include <geofis/algorithm/zoning/pair/zone_pair_distance_equal.hpp>

namespace geofis {

/*

@startuml

start

:sort **zone_pairs** by minimum distance;
note right
	std::list<>::sort is guaranteed to be stable.
	see [[http://www.sgi.com/tech/stl/List.html]]
end note

:copy all **zone_pairs** iterators equals to minimum distance to the **output**;

end

@enduml

*/

struct minimum_aggregation {

	template <class ZonePair, class OutputIterator> void operator()(std::list<ZonePair> &zone_pairs, OutputIterator output) const {

		typedef typename std::list<ZonePair>::iterator zone_pair_iterator_type;

		zone_pairs.sort(zone_pair_distance_less());
		zone_pair_iterator_type zone_pair_iterator = boost::begin(zone_pairs);
		zone_pair_iterator_type end_zone_pair_iterator = boost::end(zone_pairs);
	    for ( ; (zone_pair_iterator != end_zone_pair_iterator) && (zone_pair_distance_equal()(*zone_pair_iterator, zone_pairs.front())) ; ++zone_pair_iterator)
	    	*output = zone_pair_iterator;
	}
};

} // namespace geofis

#endif /* MINIMUM_AGGREGATION_HPP_ */
