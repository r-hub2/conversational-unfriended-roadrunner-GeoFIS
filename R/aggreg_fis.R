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

#' @title Class "AggregFis"
#' @name AggregFis
#' @docType class
#' @description The Fis aggregation operator to be used in [Fusion]
#'
#' @slot fis [Fis] object, The Fis to be used in the aggregation operator
#' @slot output_index [integer] value, The index (1-based index) of the output in the Fis to be used in the aggregation
#'
#' @importFrom utils packageVersion
#'
#' @seealso {
#' [NewAggregFis]
#'
#' [Aggregation using linguistic rules](https://www.geofis.org/en/documentation-en/data-fusion/#linguistic-rules)
#' }
#'
if (packageVersion("FisPro") == "1.0") {
  # fis slot class should be "Rcpp_fis", but that crashes crash the jvm 32bits, I don't know why.
  # envRefClass is the base class of Rcpp_fis
  # see getClass("Rcpp_fis") or http://finzi.psych.upenn.edu/R/library/Rcpp/html/CppConstructor-class.html
  # envRefClass do not crash the jvm 32bits
  setClass("AggregFis", slots = c(fis = "envRefClass", output_index = "numeric"))
} else {
  setClass("AggregFis", slots = c(fis = "Rcpp_Fis", output_index = "numeric"))
}

.CheckAggregFisRange <- function(type, name, range) {
  if ((range["min"] < 0) || (range["max"] > 1)) {
    stop(sprintf("%s '%s' must be in range [0, 1]", type, name))
  }
}

.CheckAggregFisInput <- function(input) {
  .CheckAggregFisRange("input", input$name, input$range())
}

.CheckAggregFisOutput <- function(output) {
  .CheckAggregFisRange("output", output$name, output$range())
}

setGeneric(name = ".CheckAggregFisConclusion", def = function(output, conclusion) {
  standardGeneric(".CheckAggregFisConclusion")
})

setMethod(f = ".CheckAggregFisConclusion", signature = c("Rcpp_FisOutCrisp", "numeric"), definition = function(output, conclusion) {
  if ((conclusion < 0) || (conclusion > 1)) {
    stop(sprintf("crisp output '%s' conclusion must be in range [0, 1]", output$name))
  }
})

setMethod(f = ".CheckAggregFisConclusion", signature = c("Rcpp_FisOutFuzzy", "numeric"), definition = function(output, conclusion) {})

#' @importFrom foreach foreach
#' @importFrom foreach %do%
.CheckAggregFisRule <- function(rule, outputs) {
  output <- conclusion <- NULL # for devtools::check() "no visible binding for global variable"
  foreach(output = outputs, conclusion = rule$conclusions) %do% {
    .CheckAggregFisConclusion(output, conclusion)
  }
}

.CheckAggregFis <- function(fis, output_index) {
  for (input in fis$get_inputs()) {
    .CheckAggregFisInput(input)
  }
  if (fis$output_size() < output_index) {
    stop(sprintf("output '%d' not found", output_index))
  }
  for (output in fis$get_outputs()) {
    .CheckAggregFisOutput(output)
  }
  for (rule in fis$get_rules()) {
    .CheckAggregFisRule(rule, fis$get_outputs())
  }
}

#' @importFrom methods callNextMethod
setMethod("initialize", "AggregFis", function(.Object, ...) {
  .Object <- callNextMethod()
  if (packageVersion("FisPro") != "1.0") {
    .CheckAggregFis(.Object@fis, .Object@output_index)
  }
  return(.Object)
})

#' @title Create object of class "AggregFis"
#' @name NewAggregFis
#' @description Function to create an aggregation operator of class [AggregFis] to be used in [Fusion]
#'
#' @param fis [Fis] object, The Fis to be used in the aggregation operator
#' @param output_index [integer] value, The index (1-based index) of the output in the Fis to be used in the aggregation (the default is 1)
#'
#' @return [AggregFis] object
#'
#' @seealso [Aggregation using linguistic rules](https://www.geofis.org/en/documentation-en/data-fusion/#linguistic-rules)
#'
#' @export
NewAggregFis <- function(fis, output_index = 1) {
  return(new("AggregFis", fis = fis, output_index = output_index))
}

setMethod(f = ".CheckAggreg", signature = c("AggregFis", "integer"), definition = function(aggreg, aggregate_length) {
  if (aggreg@fis$input_size() != aggregate_length) {
    stop("the Fis must have the same number of inputs as the aggregate nodes")
  }
})

#' @importFrom data.tree Aggregate
setMethod(f = "Aggreg", signature = "AggregFis", definition = function(object, node, attribute) {
  return(Aggregate(node, attribute, aggFun = function(x) object@fis$infer_output(x, object@output_index)))
})

#' @export
toString.AggregFis <- function(x, ...) {
  return(sprintf("fis(\"%s\", %d)", x@fis$name, x@output_index))
}
