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

#'
#' @importFrom methods is .hasSlot
#'
.is_spatial_data_frame <- function(x) {
  return(is(x, "Spatial") && .hasSlot(x, "data"))
}

###############################################################################

setMethod(f = ".transform_data_frame", signature = "SpatialPointsDataFrame", definition = function(x, f) {
  return(SpatialPointsDataFrame(x@coords, .transform_data_frame(x@data, f), proj4string = rebuild_CRS(x@proj4string), match.ID = FALSE))
})

setMethod(f = ".transform_data_frame", signature = "SpatialMultiPointsDataFrame", definition = function(x, f) {
  return(SpatialMultiPointsDataFrame(x@coords, .transform_data_frame(x@data, f), proj4string = rebuild_CRS(x@proj4string), match.ID = FALSE))
})

setMethod(f = ".transform_data_frame", signature = "SpatialPolygonsDataFrame", definition = function(x, f) {
  Sr <- SpatialPolygons(x@polygons, proj4string = rebuild_CRS(x@proj4string))
  return(SpatialPolygonsDataFrame(Sr, .transform_data_frame(x@data, f), match.ID = FALSE))
})

setMethod(f = ".transform_data_frame", signature = "SpatialLinesDataFrame", definition = function(x, f) {
  sl <- SpatialLines(x@lines, proj4string = rebuild_CRS(x@proj4string))
  return(SpatialLinesDataFrame(sl = sl, .transform_data_frame(x@data, f), match.ID = FALSE))
})

setMethod(f = ".transform_data_frame", signature = "SpatialPixelsDataFrame", definition = function(x, f) {
  return(SpatialPixelsDataFrame(x@coords, .transform_data_frame(x@data, f), proj4string = rebuild_CRS(x@proj4string)))
})

setMethod(f = ".transform_data_frame", signature = "SpatialGridDataFrame", definition = function(x, f) {
  return(SpatialGridDataFrame(x@grid, .transform_data_frame(x@data, f), proj4string = rebuild_CRS(x@proj4string)))
})
