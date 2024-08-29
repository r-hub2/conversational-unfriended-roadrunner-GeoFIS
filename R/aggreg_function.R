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

#' @title Class "AggregFunction"
#' @name AggregFunction
#' @docType class
#' @description The functional aggregation operator to be used in [Fusion]
#'
#' @slot func Function, The function used for the aggregation
#'
#' @seealso [NewAggregFunction]
#'
setClass("AggregFunction", slots = c(func = "function", func.name = "character"))

#' @title Create object of class "AggregFunction"
#' @name NewAggregFunction
#' @description Function to create an aggregation operator of class [AggregFunction] to be used in [Fusion]
#'
#' @param func The function to be used for the aggregation
#'
#' @export
NewAggregFunction <- function(func) {
  return(new("AggregFunction", func = func, func.name = deparse(substitute(func))))
}

setMethod(f = ".CheckAggreg", signature = c("AggregFunction", "integer"), definition = function(aggreg, aggregate_length) {})

setMethod(f = "Aggreg", signature = "AggregFunction", definition = function(object, node, attribute) {
  return(Aggregate(node, attribute, aggFun = object@func))
})

#' @export
toString.AggregFunction <- function(x, ...) {
  if (length(x@func.name)) {
    return(x@func.name)
  } else if (isS4(x@func)) {
    return(x@func@generic)
  } else if (is.primitive(x@func)) {
    func <- deparse(x@func)
    m <- gregexpr("(?<=\\.Primitive\\(\\\").*(?=\\\")", func, perl = TRUE)
    return(regmatches(func, m)[[1]])
  } else {
    # cannot find function name
    return("function(x)")
  }
}
