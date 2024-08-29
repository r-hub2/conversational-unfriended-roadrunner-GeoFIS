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

#' @title The "Minimum" distance
#' @name MinimumDistance
#' @docType methods
#' @description Function to create a "Minimum" distance\cr
#' To be used with the [Zoning] `zone_distance` field
#'
#' @return Minimum distance object
#'
#' @export
MinimumDistance <- function() {
  return(minimum_wrapper$new())
}

#' @title The "Maximum" distance
#' @name MaximumDistance
#' @docType methods
#' @description Function to create a "Maximum" distance\cr
#' To be used with the [Zoning] `zone_distance` field
#'
#' @return Maximum distance object
#'
#' @export
MaximumDistance <- function() {
  return(maximum_wrapper$new())
}

#' @title The "Mean" distance
#' @name MeanDistance
#' @docType methods
#' @description Function to create a "Mean" distance\cr
#' To be used with the [Zoning] `zone_distance` field
#'
#' @return Mean distance object
#'
#' @export
MeanDistance <- function() {
  return(mean_wrapper$new())
}

#' @title The "Euclidean" distance
#' @name EuclideanDistance
#' @docType methods
#' @description Function to create an "Euclidean" distance\cr
#' To be used with the [Zoning] `combine_distance` or `attribute_distance` field
#'
#' @return Euclidean distance object
#'
#' @export
EuclideanDistance <- function() {
  return(euclidean_wrapper$new())
}

#' @title The "Minkowski" distance
#' @name MinkowskiDistance
#' @docType methods
#' @description Function to create a "Minkowski" distance\cr
#' To be used with the [Zoning] `combine_distance` field
#'
#' @param power [numeric] value, The power of the Minkowski distance\cr
#' The default value is 2 (equivalent to euclidean distance)
#' @return Minkowski distance object
#'
#' @export
MinkowskiDistance <- function(power = 2) {
  return(minkowski_wrapper$new(power))
}

#' @title The "Fuzzy" distance
#' @name FuzzyDistance
#' @docType methods
#' @description Function to create a "Fuzzy" distance\cr
#' The fuzzy distance function is based on a fuzzy partition that allows for integrating expert knowledge into distance calculations\cr
#' To be used with the [Zoning] `attribute_distance` field
#'
#' @param fisin [FisIn] object, The partition used for the fuzzy distance (must be a standardized fuzzy partition)
#' @return Fuzzy distance object
#'
#' @references {
#' \insertRef{Infsci13}{GeoFIS}
#'
#' \insertRef{fuzzieee13}{GeoFIS}
#' }

#' @export
FuzzyDistance <- function(fisin) {
  if (!fisin$is_standardized()) {
    stop("fisin is not a standardized fuzzy partition")
  }
  return(fuzzy_wrapper$new(fisin))
}

#' @description S3 overload of [rep] function to replicate elements of Euclidean distance
#'
#' @param x Euclidean distance object, The object to replicate
#' @param times [integer] value, Number of times to repeat euclidean_distance object
#'
#' @noRd
#' @export
rep.Rcpp_euclidean_wrapper <- function(x, ...) {
  args <- list(...)
  return(lapply(1:args$times, FUN = function(dummy) x))
}
