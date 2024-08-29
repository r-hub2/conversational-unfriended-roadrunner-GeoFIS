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
#ifndef FUSION_MAP_RANGE_HPP_
#define FUSION_MAP_RANGE_HPP_

#include <boost/range.hpp>
#include <geofis/algorithm/zoning/fusion/map/fusion_map_iterator.hpp>

namespace geofis {

template <class FusionRange> class fusion_map_range;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FusionRange> struct fusion_map_range_traits {

	typedef typename boost::range_iterator<FusionRange>::type fusion_iterator_type;
	typedef typename fusion_map_iterator_traits<fusion_iterator_type>::fusion_map_iterator_type fusion_map_iterator_type;
	typedef boost::iterator_range<fusion_map_iterator_type> base_type;
	typedef fusion_map_range<FusionRange> fusion_map_range_type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FusionRange> class fusion_map_range : public fusion_map_range_traits<FusionRange>::base_type {

	typedef typename fusion_map_range_traits<FusionRange>::fusion_map_iterator_type fusion_map_iterator_type;
	typedef typename fusion_map_range_traits<FusionRange>::base_type base_type;

public:
	fusion_map_range(const fusion_map_iterator_type &begin_step, const fusion_map_iterator_type &end_step) : base_type(begin_step, end_step) {}
	//template <class ZoneRange> fusion_map_range(FusionRange &fusions, size_t begin, size_t end, const ZoneRange &zones, bool compute_zones = false) : base_type(fusion_map_iterator_type(fusions, zones, compute_zones), boost::next(fusion_map_iterator_type(fusions, zones, compute_zones), end)) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class FusionRange, class ZoneRange> typename fusion_map_range_traits<FusionRange>::fusion_map_range_type make_fusion_map_range(FusionRange &fusions, size_t begin, size_t end, const ZoneRange &zones, bool compute_zones = false) {

	typedef typename fusion_map_range_traits<FusionRange>::fusion_map_iterator_type fusion_map_iterator_type;

	UTIL_REQUIRE(begin <= end);
	fusion_map_iterator_type begin_step = boost::next(fusion_map_iterator_type(fusions, zones, compute_zones), begin);
	//fusion_map_iterator_type end_step = boost::next(begin_step, end - begin);

	//return fusion_map_range<FusionRange>(fusions, begin, end, zones, compute_zones);
	return fusion_map_range<FusionRange>(begin_step, boost::next(begin_step, end - begin));
}

} // namespace geofis

#endif /* FUSION_MAP_RANGE_HPP_ */
