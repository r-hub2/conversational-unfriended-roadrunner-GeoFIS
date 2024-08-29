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

#' @title Class "Fusion"
#' @name Fusion
#' @docType class
#' @description The main class to perform data fusion\cr
#' More information is available in the vignette "Data Fusion with GeoFIS"
#'
#' @importFrom R6 R6Class
#' @import sp
#' @import FisPro
#' @importFrom data.tree Node
#'
#' @references {
#' \insertRef{gistam20}{GeoFIS}
#'
#' \insertRef{denys20-ascii}{GeoFIS}
#' }
#'
#' @seealso {
#' [NewFusion]
#'
#' [Data Fusion documentation](https://www.geofis.org/en/documentation-en/data-fusion/)
#' }
#'
#' @examples
#' # more information about this example in the vignette "Data Fusion with GeoFIS"
#' # section "Learning illustration"
#'
#' library(GeoFIS)
#'
#' data(fusion_cars)
#'
#' fusion <- NewFusion(fusion_cars)
#' a <- NewFusionInput("a", NewMfTrapezoidalInf(4, 20), "A")
#' v <- NewFusionInput("v", NewMfTrapezoidalSup(100, 500), "V")
#' s <- NewFusionInput("s", NewMfTrapezoidalSup(120, 220), "S")
#' c <- NewFusionInput("c", NewMfTrapezoidalInf(6, 16), "C")
#' owa_aggreg <- NewFusionAggreg("score", NewAggregOwa(c(1, 0, 0, 0)), a, v, s, c)
#' fusion$aggregate <- owa_aggreg
#' fusion$perform()
#' score <- fusion$output()["score"]
#' print(score)
#' @export
Fusion <- R6Class("Fusion",
  cloneable = FALSE,
  private = list(
    .source = NULL,
    .aggregate = NULL,
    .output = NULL
  ),
  active = list(
    #' @field aggregate [Node] object, or a [list] of [Node], The node(s) to aggregate
    aggregate = function(aggregate) {
      if (missing(aggregate)) {
        return(private$.aggregate)
      } else {
        if (is(aggregate, "list")) {
          if (!all(as.logical(lapply(aggregate, function(x) is(x, "Node"))))) stop("aggregate must be a list of 'Node' object")
        } else {
          if (!is(aggregate, "Node")) stop("aggregate must be a 'Node' object")
        }
        private$.aggregate <- aggregate
      }
    }
  ),
  public = list(
    #' @description The constructor to build an object of class [Fusion].
    #' @param source [data.frame] or [Spatial]*DataFrame object of [sp] package\cr
    #' Keep only numeric attributes
    initialize = function(source) {
      if (missing(source) || (!is.data.frame(source) && !.is_spatial_data_frame(source))) stop("the source dataset must be a 'data.frame' or 'Spatial*DataFrame' object")
      private$.source <- .filter_data_frame(source, is.numeric)
    },

    #' @description Perform the data fusion
    perform = function() {
      private$.output <- .aggregate_node(private$.source, private$.aggregate)
    },


    #' @description Get the output aggregated data (same object type as data source)
    #' @return [data.frame] or [Spatial]*DataFrame object
    output = function() {
      return(private$.output)
    }
  )
)

#' @title Create object of class "Fusion"
#' @name NewFusion
#' @docType methods
#' @description Function to create object of class [Fusion]
#'
#' @param ... arguments of [Fusion] constructor
#' @return [Fusion] object
#'
#' @export
NewFusion <- function(...) {
  return(Fusion$new(...))
}

#' @title Create an input node to be used in data fusion
#' @name NewFusionInput
#' @docType methods
#' @description Function to create an input node to be used in [Fusion]
#'
#' @importFrom data.tree SetFormat
#'
#' @param name [character] vector, The name of the node
#' @param mf [Mf] object, The membership function to be used to compute the satisfaction degree of the input
#' @param attribute [character] vector, The attribute name in the source dataset (default is the same as name)
#' @return [Node] object
#'
#' @seealso [From raw data to satisfaction degrees](https://www.geofis.org/en/documentation-en/data-fusion/#satisfaction-degrees)
#'
#' @export
NewFusionInput <- function(name, mf, attribute = name) {
  node <- Node$new(name)
  node$attribute <- attribute
  node$mf <- mf
  SetFormat(node, "mf", formatFun = function(x) toString(x))
  return(node)
}

#' @title Create an aggregation node to be used in data fusion
#' @name NewFusionAggreg
#' @docType methods
#' @description Function to create an aggregation node to be used in [Fusion]
#'
#' @importFrom data.tree SetFormat
#'
#' @param name [character] vector, The name of the node
#' @param aggreg Aggreg object, The aggregation operator to be used to compute the aggregation of satisfaction degrees\cr
#' must be an [AggregWam], [AggregOwa], [AggregFis] or [AggregFunction] object
#' @param ... [Node] objects, The nodes to aggregate\cr
#' can be an input node built with [NewFusionInput] or an aggregate node built with [NewFusionAggreg] for a hierarchical aggregation structure
#' @return [Node] object
#'
#' @seealso [Aggregation of the degrees](https://www.geofis.org/en/documentation-en/data-fusion/#aggregation)
#'
#' @export
NewFusionAggreg <- function(name, aggreg, ...) {
  childs <- list(...)
  .CheckAggreg(aggreg, length(childs))
  node <- Node$new(name)
  node$aggreg <- aggreg
  for (child in childs) {
    node$AddChildNode(child)
  }
  SetFormat(node, "aggreg", formatFun = function(x) toString(x))
  return(node)
}
