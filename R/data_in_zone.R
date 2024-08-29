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

.zones_accumulator <- function(zones, overlay_id, use_id) {
  overlay_index <- overlay_id[[1]]$index
  overlay_data <- overlay_id[[1]]$value
  id <- as.integer(overlay_id[[2]])
  zone_index <- ifelse(use_id, id, overlay_index)
  zone_mask <- as.integer(rownames(overlay_data))
  zones[zone_mask] <- zone_index
  return(zones)
}

#' @importFrom itertools enumerate
#' @importFrom itertools izip
#' @importFrom purrr reduce
.zones_in_map <- function(map, data, use_id) {
  overlay <- over(map, data, returnList = TRUE)
  enum_overlay <- as.list(enumerate(overlay))
  overlay_id <- as.list(izip(enum_overlay, map@data$id))
  zones <- integer(length(data))
  zones <- reduce(overlay_id, .zones_accumulator, use_id, .init = zones)
  return(zones)
}

.map_name <- function(number_of_zones) {
  paste("map", number_of_zones, sep = "")
}


#' @title Classify data in management zones
#' @name DataInZone
#' @docType methods
#' @description Classify data in management zones of maps obtained with the [Zoning] algorithms.
#'
#' @param data [SpatialPointsDataFrame] or [SpatialMultiPointsDataFrame] object, The input data.
#' @param maps [SpatialPolygonsDataFrame] object, or a [list] of [SpatialPolygonsDataFrame], The map or list of maps to process.
#' @param use_id boolean, Use the id attribute of the zone in the map if TRUE, or the order index (1-based indexed) of the zone in the map if FALSE (the default).
#' @return [SpatialPointsDataFrame] or [SpatialMultiPointsDataFrame] object depending of the type of input data. Return table of membership of each point in the zones for each map, the input data and a column for each map processed (named "map\{number of zones in the map\}") with the class of each point.
#'
#' @export
DataInZone <- function(data, maps, use_id = FALSE) {
  if (!(is(data, "SpatialPointsDataFrame") || is(data, "SpatialMultiPointsDataFrame"))) stop("data must be a 'SpatialPointsDataFrame' or 'SpatialMultiPointsDataFrame' object")
  if (is(maps, "list")) {
    if (!all(as.logical(lapply(maps, function(x) is(x, "SpatialPolygonsDataFrame"))))) stop("maps must be a list of 'SpatialPolygonsDataFrame' object")
  } else {
    if (!is(maps, "SpatialPolygonsDataFrame")) stop("maps must be a 'SpatialPolygonsDataFrame' object")
    maps <- list(maps)
  }
  zones_in_maps <- lapply(maps, .zones_in_map, data = data, use_id = use_id)
  zones_in_maps <- do.call(cbind, zones_in_maps)
  numbers_of_zones <- sapply(maps, function(map) nrow(map))
  map_names <- sapply(numbers_of_zones, .map_name)
  colnames(zones_in_maps) <- map_names
  data_in_zone <- cbind(data, zones_in_maps)
  return(data_in_zone)
}
