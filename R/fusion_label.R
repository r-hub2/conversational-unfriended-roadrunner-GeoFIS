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

#' @title Class "FusionLabel"
#' @name FusionLabel
#' @docType class
#' @description Defines the allowed labels for the [Mf]s of the fuzzy inputs or output in the [Fis] "Fusion"
#' @importFrom R6 R6Class
#'
#' @export
FusionLabel <- R6Class("FusionLabel",
  cloneable = FALSE,
  active = list(
    #' @field very_low [character] vector (read-only), The very_low label
    very_low = function() {
      return("very low")
    },

    #' @field low [character] vector (read-only), The low label
    low = function() {
      return("low")
    },

    #' @field average [character] vector (read-only), The average label
    average = function() {
      return("average")
    },

    #' @field high [character] vector (read-only), The high label
    high = function() {
      return("high")
    },

    #' @field very_high [character] vector (read-only), The very_high label
    very_high = function() {
      return("very high")
    }
  ),
  public = list(
    #' @description Get the allowed labels depending on the granularity in the [Fis]\cr
    #' for granularity 2, allowed labels are: \[low, high\]\cr
    #' for granularity 3, allowed labels are: \[low, average, high\]\cr
    #' for granularity 4, allowed labels are: \[very_low, low, high, very_high\]\cr
    #' for granularity 5, allowed labels are: \[very_low, low, average, high, very_high\]
    #' @param granularity [integer] value, The granularity of the fuzzy inputs or output in the [Fis] (value in range \[2, 5\])
    #' @return [character] vector, The allowed labels for the granularity
    get_labels = function(granularity) {
      .CheckGranularityRange(granularity)
      switch(granularity,
        return(NULL),
        return(c(self$low, self$high)),
        return(c(self$low, self$average, self$high)),
        return(c(self$very_low, self$low, self$high, self$very_high)),
        return(c(self$very_low, self$low, self$average, self$high, self$very_high)),
      )
    }
  )
)
