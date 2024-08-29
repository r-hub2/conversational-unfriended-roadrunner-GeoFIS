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
#ifndef H90F4A34E_CFD8_491B_A582_852DCF44D0AE
#define H90F4A34E_CFD8_491B_A582_852DCF44D0AE

#include <Rcpp.h>
#include <geofis/process/zoning/zoning_process.hpp>

struct merge_checker;

class zoning_wrapper {

	typedef typename geofis::zoning_process_traits::merge_type merge_type;

	Rcpp::S4 source;
	std::unique_ptr<geofis::zoning_process> zp;

	zoning_wrapper() = delete;
	zoning_wrapper(const zoning_wrapper &) = delete;
	zoning_wrapper(zoning_wrapper && ) = delete;
	zoning_wrapper& operator=(const zoning_wrapper &) = delete;
	zoning_wrapper& operator=(zoning_wrapper &&) = delete;

	void check_size_merge(const geofis::size_merge &size_merge) const;
	void check_area_merge(const geofis::area_merge &area_merge) const;
	void check_merge(const merge_type &merge) const;

	friend struct merge_checker;

public:
	zoning_wrapper(Rcpp::S4 source);
	~zoning_wrapper();

	void set_border(Rcpp::S4 border);

	void perform_voronoi();
	void release_voronoi();

	Rcpp::Nullable<Rcpp::S4> get_voronoi_map();

	size_t get_bounded_feature_size() const;

	void set_all_neighborhood();
	void set_edge_length_neighborhood(double edge_length);

	void perform_neighborhood();
	void release_neighborhood();

	Rcpp::Nullable<Rcpp::S4> get_neighborhood_map();

	void set_attribute_distances(Rcpp::List attribute_distance_list);

	void set_zone_distance(const util::maximum<double> &maximum_distance);
	void set_zone_distance(const util::minimum<double> &minimum_distance);
	void set_zone_distance(const util::mean<double> &mean_distance);

	void set_multidimensional_distance(const util::euclidean_distance<double> &euclidean_distance);
	void set_multidimensional_distance(const util::minkowski_distance<double> &minkowski_distance);

	void perform_fusion();
	void release_fusion();

	int get_fusion_size();

	void set_merge(const geofis::size_merge &size_merge);
	void set_merge(const geofis::area_merge &area_merge);

	void perform_merge();
	void release_merge();

	int get_merge_size();

	Rcpp::Nullable<Rcpp::S4> get_merge_map(size_t number_of_zones);
	Rcpp::Nullable<Rcpp::List> get_merge_maps(Rcpp::IntegerVector number_of_zones);
};

#endif // H90F4A34E_CFD8_491B_A582_852DCF44D0AE
