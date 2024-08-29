#' Build a 3X3 data source
#'
#'   +-----------+-----+
#'   ¦  1  ¦  2  ¦ 2.4 ¦
#'   +-----+-----+-----¦
#'   ¦ 1.1 ¦  9  ¦ 5.1 ¦
#'   +-----+-----+-----¦
#'   ¦ 1.3 ¦  4  ¦ 4.5 ¦
#'   +-----------------+
#'
#' @return SpatialPointsDataFrame
#'
get_source_3_3 <- function(crs) {
  coords <- get_spatial("GEOMETRYCOLLECTION(
    POINT(0.5 2.5), POINT(1.5 2.5), POINT(2.5 2.5),
    POINT(0.5 1.5), POINT(1.5 1.5), POINT(2.5 1.5),
    POINT(0.5 0.5), POINT(1.5 0.5), POINT(2.5 0.5))", crs)
  a <- c(1, 2, 2.4, 1.1, 9, 5.1, 1.3, 4, 4.5)
  return(SpatialPointsDataFrame(coords = coords, data = as.data.frame(a)))
}

#' Build a 3X3 border
#'
#' @return SpatialPolygons
#'
get_border_3_3 <- function(crs) {
  return(get_spatial("POLYGON((0 0, 0 3, 3 3, 3 0, 0 0))", crs))
}

#' Build a 2X2 data source
#'
#'   +-------+
#'   ¦ 1 ¦ 2 ¦
#'   +---+---¦
#'   ¦ 3 ¦ 4 ¦
#'   +-------+
#'
#' @return SpatialPointsDataFrame
#'
get_source_2_2 <- function(crs) {
  coords <- get_spatial("GEOMETRYCOLLECTION(
    POINT(0.5 1.5), POINT(1.5 1.5),
    POINT(0.5 0.5), POINT(1.5 0.5))", crs)
  a <- c(1, 2, 3, 4)
  return(SpatialPointsDataFrame(coords = coords, data = as.data.frame(a)))
}

#' Build a 2X2 multi points data source
#'
#'   +-------+
#'   ¦ 1 ¦ 1 ¦
#'   +---+---¦
#'   ¦ 2 ¦ 2 ¦
#'   +-------+
#'
#' @return SpatialMultiPointsDataFrame
#'
get_multi_points_source <- function(crs) {
  points1 <- get_spatial("MULTIPOINT((0.5 1.5), (1.5 1.5))", crs)
  points2 <- get_spatial("MULTIPOINT((0.5 0.5), (1.5 0.5))", crs)
  coords <- SpatialMultiPoints(coords = list(points1, points2), proj4string = rebuild_CRS(points1@proj4string))
  a <- c(1, 2)
  return(SpatialMultiPointsDataFrame(coords = coords, data = as.data.frame(a)))
}

#' Build a 2X2 border
#'
#' @return SpatialPolygons
#'
get_border_2_2 <- function(crs) {
  return(get_spatial("POLYGON((0 0, 0 2, 2 2, 2 0, 0 0))", crs))
}
