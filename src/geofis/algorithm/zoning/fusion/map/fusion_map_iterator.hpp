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
#ifndef FUSION_MAP_ITERATOR_HPP_
#define FUSION_MAP_ITERATOR_HPP_

#include <list>
#include <boost/ref.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <util/address_equal.hpp>
#include <geofis/algorithm/zoning/fusion/map/fusion_map.hpp>

namespace geofis {

template <class FusionIterator> class fusion_map_iterator;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FusionIterator> struct fusion_map_iterator_traits {

	typedef typename std::iterator_traits<FusionIterator>::value_type fusion_type;
	typedef typename fusion_type::zone_type zone_type;
	typedef boost::reference_wrapper<zone_type> zone_reference_type;
	typedef std::list<zone_reference_type> zone_reference_container_type;
	typedef fusion_map<zone_type> fusion_map_type;
	typedef fusion_map_iterator<FusionIterator> fusion_map_iterator_type;
	typedef boost::iterator_adaptor<fusion_map_iterator_type, FusionIterator, fusion_map_type, boost::use_default, fusion_map_type> base_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FusionIterator> class fusion_map_iterator : public fusion_map_iterator_traits<FusionIterator>::base_type {

	friend class boost::iterator_core_access;

	typedef typename fusion_map_iterator_traits<FusionIterator>::base_type base_type;
	typedef typename fusion_map_iterator_traits<FusionIterator>::fusion_map_type fusion_map_type;
	typedef typename fusion_map_iterator_traits<FusionIterator>::zone_reference_container_type zone_reference_container_type;

public:
	fusion_map_iterator() : compute_zones(false) {}

	template <class FusionRange, class ZoneRange> fusion_map_iterator(FusionRange &fusions, const ZoneRange &zones, bool compute_zones = false) : base_type(boost::begin(fusions)), begin(boost::begin(fusions)), end(boost::end(fusions)), zones(boost::begin(zones), boost::end(zones)), compute_zones(compute_zones) {
		UTIL_REQUIRE(boost::size(zones) == boost::size(fusions) + 1);
		increment_zones();
	}

private:
	FusionIterator begin;
	FusionIterator end;
	zone_reference_container_type zones;
	bool compute_zones;

    typename base_type::reference dereference() const {
		return fusion_map_type(zones, *base_type::base(), compute_zones);
    }

    void increment() {
    	++base_type::base_reference();
    	if(base_type::base_reference() != end)
    		increment_zones();
    }

    void decrement() {
    	if(base_type::base_reference() != begin) {
    		if(base_type::base_reference() != end) {
    			decrement_zones();
    		}
			--base_type::base_reference();
    	}
    }

	void increment_zones() {
		increment_zones(*base_type::base_reference());
	}

	template <class Fusion> void increment_zones(Fusion &fusion) {
		increment_zones(fusion.get_zone1(), fusion.get_zone2(), fusion.get_fusion());
	}

	template <class Zone> void increment_zones(const Zone &zone1, const Zone &zone2, Zone &fusion) {
		zones.erase(boost::find_if(zones, util::address_equal(zone1)));
		zones.erase(boost::find_if(zones, util::address_equal(zone2)));
		zones.push_back(boost::ref(fusion));
	}

	void decrement_zones() {
		decrement_zones(*base_type::base_reference());
	}

	template <class Fusion> void decrement_zones(Fusion &fusion) {
		decrement_zones(fusion.get_zone1(), fusion.get_zone2(), fusion.get_fusion());
	}

	template <class Zone> void decrement_zones(Zone &zone1, Zone &zone2, const Zone &fusion) {
		zones.erase(boost::find_if(zones, util::address_equal(fusion)));
		zones.push_back(boost::ref(zone1));
		zones.push_back(boost::ref(zone2));
	}
};

} // namespace geofis

#endif /* FUSION_MAP_ITERATOR_HPP_ */
