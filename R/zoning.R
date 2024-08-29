################################################################################
#
# GeoFIS R package
#
# Copyright (C) 2021 INRAE
#
# Authors:
# 	Jean-luc Lablée - INRAE
# 	Serge Guillaume - INRAE
#
# License: CeCILL v2.1
# 	https://cecill.info/licences/Licence_CeCILL_V2.1-en.html
# 	https://cecill.info/licences/Licence_CeCILL_V2.1-fr.html
#
# This software is governed by the CeCILL license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "https://cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights, and the successive licensors have only limited liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
#
################################################################################

#' @name .is_zonable
#' @description Predicate function to determine if a vector data is zonable\cr
#' The vector data is zonable if it is numeric, without missing value, and normalizable (not an unique value)
#' @param x Vector, The data
#'
#' @noRd
#' @keywords internal
.is_zonable <- function(x) {
  is.numeric(x) && !any(is.na(x)) && (length(unique(x)) != 1)
}

#' @name .convex_hull
#' @description makes the convex hull of a spatial object
#' @param x Spatial, The input object
#'
#' @noRd
#' @importFrom sf st_as_sfc st_combine st_convex_hull st_is_valid as_Spatial
#' @keywords internal
.convex_hull <- function(x) {
  return(
    st_as_sfc(x) %>%
      st_combine() %>%
      st_convex_hull() %>%
      as_Spatial()
  )
}

#' @title Class "Zoning"
#' @name Zoning
#' @docType class
#' @description The main class to perform zoning\cr
#' A complete use-case example is described in the vignette "Zoning with GeoFIS"
#'
#' @importFrom R6 R6Class
#' @import sp
#'
#' @references {
#' \insertRef{Pedroso10}{GeoFIS}
#'
#' \insertRef{Infsci13}{GeoFIS}
#'
#' \insertRef{fuzzieee13}{GeoFIS}
#' }
#'
#' @seealso {
#' [NewZoning]
#'
#' [Zoning documentation](https://www.geofis.org/en/documentation-en/zoning/)
#' }
#'
#' @export
Zoning <- R6Class("Zoning",
  cloneable = FALSE,
  private = list(
    .zonable_data = NULL,
    .border = NULL,
    .neighborhood = NULL,
    .zoning_wrapper = NULL,
    .check_zonable_data = function(source, zonable, warn) {
      if (ncol(zonable) == 0) stop("zoning data source must contains at least one zonable data")
      if (warn && (ncol(source) != ncol(zonable))) {
        not_zonable_attributes <- setdiff(colnames(source@data), colnames(zonable@data))
        warning(paste("attribute(s) [", paste(not_zonable_attributes, collapse = ", "), "] not zonable"))
      }
    },
    .check_border_crs = function(border) {
      if (!identicalCRS(border, private$.zonable_data)) stop("border must have the same coordinate system as the data source")
    },
    .check_border_overlay = function(border) {
      zonable_overlay <- over(private$.zonable_data, border)
      zonable_out_border <- sum(is.na(zonable_overlay))
      zonable_size <- nrow(private$.zonable_data)
      if (zonable_out_border == zonable_size) {
        stop("no points inside the border")
      } else if (zonable_out_border > 0) {
        warning(paste(zonable_out_border, "points are outside the border"))
      }
    },
    .check_border_geometry = function(border) {
      if (!is(border, "SpatialPolygons")) stop("the border must be a 'SpatialPolygons' object")
      if (!st_is_valid(st_as_sfc(border))) stop("the border is not a valid polygon")
      polygons_list <- border@polygons
      if (length(polygons_list) != 1) stop("the border must contain only one polygon")
      polygons <- polygons_list[[1]]
      polygon_list <- polygons@Polygons
      if (length(polygon_list) != 1) stop("the border must contain only one polygon")
    },
    .check_border = function(border) {
      private$.check_border_crs(border)
      private$.check_border_geometry(border)
      private$.check_border_overlay(border)
    },
    .check_neighborhood = function(neighborhood) {
      if (!is.numeric(neighborhood)) stop("the neighborhood must be a numeric value")
      if (neighborhood < 0) stop("the neighborhood must be a positive value")
    },
    .zonable_data_size = function() {
      return(ncol(private$.zonable_data@data))
    },
    .check_attribute_distance = function(attribute_distance) {
      if (is.null(attribute_distance)) stop("the attribute distance cannot be NULL")
      if (!is.list(attribute_distance)) {
        attribute_distance <- list(attribute_distance)
      }
      if (length(attribute_distance) != private$.zonable_data_size()) {
        stop("the number of attribute distances must be equal to the number of zonable attributes")
      }
      if (all(sapply(attribute_distance, is.null))) {
        stop("at least one attribute distance must not be NULL")
      }
    },
    .check_orphan_zones = function() {
      bounded_feature_size <- private$.zoning_wrapper$get_bounded_feature_size()
      fusion_size <- private$.zoning_wrapper$get_fusion_size()
      if (bounded_feature_size != (fusion_size + 1)) {
        private$.zoning_wrapper$release_merge()
        private$.zoning_wrapper$release_fusion()
        orphan_zones <- bounded_feature_size - fusion_size - 1
        stop(paste(orphan_zones, "zones have no neighbors, decrease the neighborhood value before perform zoning"))
      }
    }
  ),
  active = list(
    #' @field border [SpatialPolygons] object, The border used to limit the processed area, or `NULL` if the Convex Hull of data source is used\cr
    #' Only data points within the border polygon are processed\cr
    #' The default value is `NULL`
    border = function(border) {
      if (missing(border)) {
        return(private$.border)
      } else {
        if (is.null(border)) {
          private$.zoning_wrapper$set_border(.convex_hull(private$.zonable_data))
        } else {
          private$.check_border(border)
          private$.zoning_wrapper$set_border(border)
        }
        private$.border <- border
        private$.zoning_wrapper$release_merge()
        private$.zoning_wrapper$release_fusion()
        private$.zoning_wrapper$release_neighborhood()
        private$.zoning_wrapper$release_voronoi()
      }
    },

    #' @field neighborhood [numeric] value, The minimum edge length shared by two Voronoi polygons for being considered as neighbors\cr
    #' or `NULL` if all contiguous Voronoi polygons are considered as neighbors\cr
    #' The default value is `NULL`
    neighborhood = function(neighborhood) {
      if (missing(neighborhood)) {
        return(private$.neighborhood)
      } else {
        if (is.null(neighborhood)) {
          private$.zoning_wrapper$set_all_neighborhood()
        } else {
          private$.check_neighborhood(neighborhood)
          private$.zoning_wrapper$set_edge_length_neighborhood(neighborhood)
        }
        private$.neighborhood <- neighborhood
        private$.zoning_wrapper$release_merge()
        private$.zoning_wrapper$release_fusion()
        private$.zoning_wrapper$release_neighborhood()
      }
    },

    #' @field attribute_distance [list] of Distance object (write-only), The functions used to compute the distance between two data points in the attribute space\cr
    #' The length of the list must be equal to the number of zonable attributes, the distance objects are treated in the order of zonable attributes\cr
    #' In case of a single attribute into the zonable dataset, the [list] is optional and a single Distance object can be provided\cr
    #' Allowed distance objects: [EuclideanDistance], [FuzzyDistance] or `NULL` if the attribute should not be used in the zoning process\cr
    #' The default value is a [list] of [EuclideanDistance]\cr
    #' See [Zoning documentation main parameters](https://www.geofis.org/en/documentation-en/zoning/#main-parameters) univariate distance\cr
    attribute_distance = function(attribute_distance) {
      private$.check_attribute_distance(attribute_distance)
      if (!is.list(attribute_distance)) {
        attribute_distance <- list(attribute_distance)
      }
      private$.zoning_wrapper$set_attribute_distances(attribute_distance)
      private$.zoning_wrapper$release_merge()
      private$.zoning_wrapper$release_fusion()
    },

    #' @field combine_distance Distance object (write-only), The function used to combine attribute distances in case of multivariate zoning\cr
    #' Allowed distance objects: [EuclideanDistance] or [MinkowskiDistance]\cr
    #' The default value is [EuclideanDistance]
    #' See [Zoning documentation main parameters](https://www.geofis.org/en/documentation-en/zoning/#main-parameters) multivariate combination\cr
    combine_distance = function(combine_distance) {
      private$.zoning_wrapper$set_combine_distance(combine_distance)
      private$.zoning_wrapper$release_merge()
      private$.zoning_wrapper$release_fusion()
    },

    #' @field zone_distance Distance object (write-only), The function used to compute the distance between 2 zones\cr
    #' Allowed distance objects: [MaximumDistance], [MinimumDistance] or [MeanDistance]\cr
    #' The default value is [MaximumDistance]\cr
    #' The pair of zones to be merged are those for which the `zone_distance` is minimum.\cr
    #' See [Zoning documentation main parameters](https://www.geofis.org/en/documentation-en/zoning/#main-parameters) between zone distance\cr
    zone_distance = function(zone_distance) {
      private$.zoning_wrapper$set_zone_distance(zone_distance)
      private$.zoning_wrapper$release_merge()
      private$.zoning_wrapper$release_fusion()
    },

    #' @field smallest_zone Smallest zone object (write-only), This criterion is used to determine the smallest size for a zone (number of points or area) to be kept in the final map\cr
    #' Allowed Smallest zone objects: [ZoneSize] or [ZoneArea]\cr
    #' The default value is [ZoneSize] with 1 point
    smallest_zone = function(smallest_zone) {
      private$.zoning_wrapper$set_merge(smallest_zone)
      private$.zoning_wrapper$release_merge()
    }
  ),
  public = list(
    #' @description Constructor, create a new instance of [Zoning]
    #' @param source [SpatialPointsDataFrame] or [SpatialMultiPointsDataFrame] object, The data source
    #' @param warn [logical] value, Show warnings if TRUE, default value is TRUE
    initialize = function(source, warn = TRUE) {
      if (missing(source) || !(is(source, "SpatialPointsDataFrame") || is(source, "SpatialMultiPointsDataFrame"))) stop("zoning data source must be a 'SpatialPointsDataFrame' or 'SpatialMultiPointsDataFrame' object")
      if (!is.projected(source)) stop("zoning data source must be in projected coordinate reference system")
      if (is(source, "SpatialMultiPointsDataFrame")) {
        source <- as(source, "SpatialPointsDataFrame")
      }
      private$.zonable_data <- .filter_data_frame(source, .is_zonable)
      private$.check_zonable_data(source, private$.zonable_data, warn)
      private$.zoning_wrapper <- new(zoning_wrapper, private$.zonable_data)
      private$.zoning_wrapper$set_border(.convex_hull(private$.zonable_data))
      private$.zoning_wrapper$set_all_neighborhood()
      private$.zoning_wrapper$set_attribute_distances(rep(EuclideanDistance(), times = private$.zonable_data_size()))
      private$.zoning_wrapper$set_zone_distance(MaximumDistance())
      private$.zoning_wrapper$set_combine_distance(EuclideanDistance())
      private$.zoning_wrapper$set_merge(ZoneSize(1))
    },

    #' @description Get the zonable data\cr
    #' Keep only the attributes that can be used in the zoning process, meaning numeric atributes, without missing values and with a range that is not limited to a unique value\cr
    #' The last condition is required by the min-max standardization process
    #' @return [SpatialPointsDataFrame] object
    zonable_data = function() {
      return(private$.zonable_data)
    },

    #' @description Compute the Voronoi diagram
    perform_voronoi = function() {
      private$.zoning_wrapper$perform_voronoi()
    },

    #' @description Get the Voronoi map
    #' @return [SpatialPolygons] object
    voronoi_map = function() {
      return(private$.zoning_wrapper$get_voronoi_map())
    },

    #' @description Identify adjacent polygons in the voronoi tesselation
    perform_neighborhood = function() {
      private$.zoning_wrapper$perform_voronoi()
      private$.zoning_wrapper$perform_neighborhood()
    },

    #' @description Get the neighborhood map
    #' @return [SpatialLinesDataFrame] object
    neighborhood_map = function() {
      return(private$.zoning_wrapper$get_neighborhood_map())
    },

    #' @description Perform the zoning
    perform_zoning = function() {
      private$.zoning_wrapper$perform_voronoi()
      private$.zoning_wrapper$perform_neighborhood()
      private$.zoning_wrapper$perform_fusion()
      private$.check_orphan_zones()
      private$.zoning_wrapper$perform_merge()
    },

    #' @description Get the number of maps with different number of zones available after perform zoning
    #' @return [integer] value
    map_size = function() {
      return(private$.zoning_wrapper$get_merge_size())
    },

    #' @description Get the map corresponding to a number of zones
    #' @param number_of_zones [integer] value, The number of zones in the map
    #' @return [SpatialPolygonsDataFrame] object
    map = function(number_of_zones) {
      return(private$.zoning_wrapper$get_merge_map(number_of_zones))
    },

    #' @description Get the maps corresponding to a number of zones
    #' @param number_of_zones [integer] vector, The number of zones in each map
    #' @return [list] of [SpatialPolygonsDataFrame] object
    maps = function(number_of_zones) {
      return(private$.zoning_wrapper$get_merge_maps(number_of_zones))
    }
  )
)

#' @title Create object of class "Zoning"
#' @name NewZoning
#' @docType methods
#' @description Function to create object of class [Zoning]
#'
#' @param ... arguments of [Zoning] constructor
#' @return [Zoning] object
#'
#' @export
NewZoning <- function(...) {
  return(Zoning$new(...))
}
