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

#' @title Class "AggregOwa"
#' @name AggregOwa
#' @docType class
#' @description The OWA aggregation operator to be used in [Fusion]
#'
#' @slot weights [numeric] vector, The weights of the OWA aggregation operator (the sum of the weights must be equal to 1 without negative values)
#'
#' @seealso {
#' [NewAggregOwa]
#'
#' [Aggregation using numerical operators](https://www.geofis.org/en/documentation-en/data-fusion/#numerical-operators)
#' }
#'
setClass("AggregOwa", slots = c(weights = "numeric"))

#' @importFrom methods callNextMethod
setMethod("initialize", "AggregOwa", function(.Object, ...) {
  .Object <- callNextMethod()
  .CheckAggregWeights(.Object@weights)
  return(.Object)
})

#' @title Create object of class "AggregOwa"
#' @name NewAggregOwa
#' @description Function to create an aggregation operator of class [AggregOwa] to be used in [Fusion]
#'
#' @param weights [numeric] vector, The weights of the OWA aggregation operator (the sum of the weights must be equal to 1 without negative values)
#'
#' @seealso [Aggregation using numerical operators](https://www.geofis.org/en/documentation-en/data-fusion/#numerical-operators)
#'
#' @export
NewAggregOwa <- function(weights) {
  return(new("AggregOwa", weights = weights))
}

setMethod(f = ".CheckAggreg", signature = c("AggregOwa", "integer"), definition = function(aggreg, aggregate_length) {
  if (length(aggreg@weights) != aggregate_length) {
    stop("the OWA weigths must have the same length as the aggregate nodes")
  }
})

.ComputeOwa <- function(x, weights) {
  return(sum(sort(x) * weights))
}

#' @importFrom data.tree Aggregate
setMethod(f = "Aggreg", signature = "AggregOwa", definition = function(object, node, attribute) {
  return(Aggregate(node, attribute, aggFun = function(x) .ComputeOwa(x, object@weights)))
})

#' @export
toString.AggregOwa <- function(x, ...) {
  return(sprintf("owa(%s)", paste(x@weights, collapse = ", ")))
}
