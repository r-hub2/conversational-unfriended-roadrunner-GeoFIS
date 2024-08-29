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

#' @description Rounding weights preserving their sum to 1
#' https://stackoverflow.com/questions/32544646/round-vector-of-numerics-to-integer-while-preserving-their-sum
#'
#' @param x [numeric] vector, input values
#' @param digits [integer] value, number of digits to which number should be rounded
#'
#' @return [numeric] vector, rounded values
#'
#' @importFrom utils tail
#'
#' @noRd
#' @keywords internal
.get_rounded_weights <- function(x, digits) {
  up <- 10^digits
  x <- x * up
  y <- floor(x)
  indices <- tail(order(x - y), up - sum(y))
  y[indices] <- y[indices] + 1
  return(y / up)
}

#' @description Compute the learning weights (the sum of weights equals 1)
#'
#' @param a [numeric] matrix, input data with m rows and n columns
#' @param b [numeric] vector, target value of length m
#'
#' @return [numeric] vector, weights
#'
#' @importFrom nnls nnls
#'
#' @noRd
#' @keywords internal
.compute_learning_weights <- function(a, b) {
  if (!is.matrix(a)) stop("'a' must be a matrix")
  if (!is.vector(b)) stop("'b' must be a vector")
  if (length(b) != nrow(a)) stop(paste("'b' vector length must be", nrow(a)))
  a <- a - b
  b <- c(rep(0, nrow(a)), 1)
  a <- rbind(a, rep(1, ncol(a)))
  r <- nnls(a, b)
  return(r$x / sum(r$x))
}

setGeneric(name = ".check_range", def = function(x) {
  standardGeneric(".check_range")
})

setMethod(f = ".check_range", signature = "numeric", definition = function(x) {
  if ((min(x) < 0) || (max(x) > 1) || any(is.na(x))) {
    stop("data must be in range [0, 1]")
  }
})

setMethod(f = ".check_range", signature = "matrix", definition = function(x) {
  foreach(col = 1:ncol(x)) %do% .check_range(x[, col])
})

setMethod(f = ".check_range", signature = "data.frame", definition = function(x) {
  .check_range(as.matrix(x))
})

#' @title Learn the WAM weights
#' @name LearnWamWeights
#' @description Learn the WAM weights using a non-negative least-square optimization method with the constraint that the sum of weights must be equal to 1.\cr
#' More information is available in the vignette "Data Fusion with GeoFIS", section "Learning illustration".
#'
#' @param data [data.frame] or [numeric] matrix, The input data (all columns must be in range \[0, 1\])
#' @param target [numeric] vector, The target data (must be in range \[0, 1\])
#' @param digits [integer] value, The number of digits to which weights are to be rounded (default is 3)
#'
#' @return [numeric] vector, The WAM weights
#' @export
LearnWamWeights <- function(data, target, digits = 3) {
  .check_range(data)
  .check_range(target)
  weights <- .compute_learning_weights(as.matrix(data), target)
  return(.get_rounded_weights(weights, digits))
}

#' @title Learn the OWA weights
#' @name LearnOwaWeights
#' @description Learn the OWA weights using a non-negative least-square optimization method with the constraint that the sum of weights must be equal to 1. The input values are previously sorted in increasing order. The resulting weights are given from min to max.
#' More information is available in the vignette "Data Fusion with GeoFIS", section "Learning illustration".
#'
#' @param data [data.frame] or [numeric] matrix, The input data (all columns must be in range \[0, 1\])
#' @param target [numeric] vector, The target data (must be in range \[0, 1\])
#' @param digits [integer] value, The number of digits to which weights are to be rounded (default is 3)
#'
#' @return [numeric] vector, The OWA weights
#' @export
LearnOwaWeights <- function(data, target, digits = 3) {
  .check_range(data)
  .check_range(target)
  weights <- .compute_learning_weights(t(apply(data, 1, sort)), target)
  return(.get_rounded_weights(weights, digits))
}
