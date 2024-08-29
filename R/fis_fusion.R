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

.CheckGranularityRange <- function(granularity) {
  if (!((granularity >= 2) && (granularity <= 5))) {
    stop("granularity must be in range [2, 5]")
  }
}

.CheckGranularities <- function(granularities) {
  for (granularity in granularities) {
    .CheckGranularityRange(granularity)
  }
}

setGeneric(name = ".CheckConclusions", def = function(conclusion_length, conclusions) {
  standardGeneric(".CheckConclusions")
})

.CheckConclusionRange <- function(conclusion) {
  if (!((conclusion >= 0) && (conclusion <= 1))) {
    stop("conclusion must be in range [0, 1]")
  }
}

setMethod(f = ".CheckConclusions", signature = c("missing", "numeric"), definition = function(conclusion_length, conclusions) {
  for (conclusion in conclusions) {
    .CheckConclusionRange(conclusion)
  }
})

.CheckUniqueLabels <- function(labels) {
  granularity <- length(labels)
  if (!((granularity >= 2) && (granularity <= 5))) {
    stop("conclusions must have between 2 to 5 unique labels")
  }
  fl <- FusionLabel$new()
  allowed_labels <- fl$get_labels(granularity)
  if (!all(sapply(labels, FUN = function(label) label %in% allowed_labels))) {
    stop(paste0("conclusion allowed labels are [", paste(allowed_labels, collapse = ", "), "]"))
  }
}

setMethod(f = ".CheckConclusions", signature = c("missing", "character"), definition = function(conclusion_length, conclusions) {
  .CheckUniqueLabels(unique(conclusions))
})

setMethod(f = ".CheckConclusions", signature = c("numeric", "ANY"), definition = function(conclusion_length, conclusions) {
  if (conclusion_length != length(conclusions)) {
    stop(paste0("conclusions must contains ", conclusion_length, " values"))
  }
  .CheckConclusions(conclusions = conclusions)
})

.MakeFuzzy <- function(fuzzy, granularity) {
  fl <- FusionLabel$new()
  labels <- fl$get_labels(granularity)
  mapply(fuzzy$get_mfs(), labels, FUN = function(mf, label) mf$label <- label)
  return(fuzzy)
}

.MakeInput <- function(granularity, name) {
  fisin <- NewFisIn(granularity, 0, 1)
  fisin$name <- name
  return(.MakeFuzzy(fisin, granularity))
}

.MakeCrispOutput <- function(name) {
  fisout_crisp <- NewFisOutCrisp(0, 1)
  fisout_crisp$name <- name
  return(fisout_crisp)
}

.MakeFuzzyOutput <- function(granularity, name) {
  fisout_fuzzy <- NewFisOutFuzzy(granularity, 0, 1)
  fisout_fuzzy$name <- name
  return(.MakeFuzzy(fisout_fuzzy, granularity))
}

setGeneric(name = ".MakeOutput", def = function(conclusions, name) {
  standardGeneric(".MakeOutput")
})

setMethod(f = ".MakeOutput", signature = c("numeric", "character"), definition = function(conclusions, name) {
  return(.MakeCrispOutput(name))
})

setMethod(f = ".MakeOutput", signature = c("character", "character"), definition = function(conclusions, name) {
  return(.MakeFuzzyOutput(length(unique(conclusions)), name))
})

.MakePremises <- function(granularities) {
  length <- length(granularities)
  return(do.call(cbind, lapply(1:length, FUN = function(i) {
    each <- ifelse(i == length, 1, prod(granularities[(i + 1):length]))
    times <- ifelse(i == 1, 1, prod(granularities[1:(i - 1)]))
    rep(1:granularities[i], each = each, times = times)
  })))
}

setGeneric(name = ".MakeRules", def = function(granularities, conclusions) {
  standardGeneric(".MakeRules")
})

setMethod(f = ".MakeRules", signature = c("numeric", "numeric"), definition = function(granularities, conclusions) {
  premises <- .MakePremises(granularities)
  return(lapply(1:nrow(premises), FUN = function(i) NewRule(premises[i, ], conclusions[i])))
})

setMethod(f = ".MakeRules", signature = c("numeric", "character"), definition = function(granularities, conclusions) {
  premises <- .MakePremises(granularities)
  fl <- FusionLabel$new()
  labels <- fl$get_labels(length(unique(conclusions)))
  return(lapply(1:nrow(premises), FUN = function(i) NewRule(premises[i, ], which(labels == conclusions[i]))))
})

#' @title Create object of class "Fis" to be used in data fusion
#' @name NewFisFusion
#' @docType methods
#' @description Function to create object of class [Fis] to be used in [AggregFis]
#'
#' @param fis_name [character] vector, The name of the Fis
#' @param input_names [character] vector, The Fis inputs names
#' @param input_granularities [integer] vector, The granularity (number of membership functions) for each Fis input (granularity must be in range \[2, 5\])
#' @param output_name [character] vector, The name of the Fis output
#' @param output_conclusions [numeric] or [character] vector, The conclusions of the rules in the Fis\cr
#' the rules are generated according to the granularity of each input, in the lexicographic order of inputs Mfs\cr
#' (`prod(input_granularities)` rules are generated)\cr
#' if [numeric] vector, a crisp output [FisOutCrisp] will be added to the Fis (all output conclusions must be be in range \[0, 1\])\cr
#' if [character] vector, a fuzzy output [FisOutFuzzy] will be added to the Fis, the output_conclusions contains the labels of Mfs in the fuzzy output (labels defined on [FusionLabel])\cr
#' the length of output_conclusions must be equal to the number of generated rules.
#' @return [Fis] object
#'
#' @seealso [Aggregation using linguistic rules](https://www.geofis.org/en/documentation-en/data-fusion/#linguistic-rules)
#'
#' @export
NewFisFusion <- function(fis_name, input_names, input_granularities, output_name, output_conclusions) {
  .CheckGranularities(input_granularities)
  .CheckConclusions(prod(input_granularities), output_conclusions)
  fis <- NewFis()
  fis$name <- fis_name
  inputs <- mapply(.MakeInput, input_granularities, input_names)
  for (input in inputs) {
    fis$add_input(input)
  }
  output <- .MakeOutput(output_conclusions, output_name)
  fis$add_output(output)
  rules <- .MakeRules(input_granularities, output_conclusions)
  for (rule in rules) {
    fis$add_rule(rule)
  }
  return(fis)
}
