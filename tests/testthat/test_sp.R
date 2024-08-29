context("Spatial")

sp_crs <- CRS("epsg:4326")

get_points <- function() {
  return(get_spatial(
    "GEOMETRYCOLLECTION(
    POINT(0 0), POINT(0 1),
    POINT(1 0), POINT(1 1))",
    sp_crs
  ))
}

get_multi_points <- function() {
  points1 <- get_spatial("MULTIPOINT((0 0), (0 1))", sp_crs)
  points2 <- get_spatial("MULTIPOINT((1 0), (1 1))", sp_crs)
  points3 <- get_spatial("MULTIPOINT((2 0), (2 1))", sp_crs)
  points4 <- get_spatial("MULTIPOINT((3 0), (3 1))", sp_crs)
  return(SpatialMultiPoints(coords = list(points1, points2, points3, points4), proj4string = rebuild_CRS(points1@proj4string)))
}

get_polygons <- function() {
  return(get_spatial_polygons(
    "GEOMETRYCOLLECTION(
    POLYGON((0 0, 0 1, 1 1, 1 0, 0 0)),
    POLYGON((1 0, 1 1, 2 1, 2 0, 1 0)),
    POLYGON((2 0, 2 1, 3 1, 3 0, 2 0)),
    POLYGON((3 0, 3 1, 4 1, 4 0, 3 0)))",
    sp_crs
  ))
}

get_lines <- function() {
  return(get_spatial_lines(
    "GEOMETRYCOLLECTION(
    LINESTRING(0 0, 0 1),
    LINESTRING(1 0, 1 1),
    LINESTRING(2 0, 2 1),
    LINESTRING(3 0, 3 1))",
    sp_crs
  ))
}

get_grid <- function() {
  return(GridTopology(cellcentre.offset = c(0, 0), cellsize = c(1, 1), cells.dim = c(2, 2)))
}

get_data_frame <- function() {
  a <- as.integer(c(0, 1, 2, 3))
  b <- c("a", "b", "c", "d")
  c <- c(4, 5, 6, 7)
  return(data.frame(a, b, c))
}

get_points_data_frame <- function() {
  return(SpatialPointsDataFrame(get_points(), get_data_frame()))
}

get_multi_points_data_frame <- function() {
  return(SpatialMultiPointsDataFrame(coords = get_multi_points(), data = get_data_frame()))
}

get_polygons_data_frame <- function() {
  return(SpatialPolygonsDataFrame(get_polygons(), data = get_data_frame()))
}

get_lines_data_frame <- function() {
  return(SpatialLinesDataFrame(get_lines(), data = get_data_frame()))
}

get_pixels_data_frame <- function() {
  return(SpatialPixelsDataFrame(get_points(), data = get_data_frame()))
}

get_grid_data_frame <- function() {
  return(SpatialGridDataFrame(get_grid(), data = get_data_frame(), proj4string = CRS("epsg:4326")))
}

###############################################################################

get_expected_data_frame <- function() {
  a <- as.integer(c(0, 1, 2, 3))
  c <- c(4, 5, 6, 7)
  return(data.frame(a, c))
}

get_expected_points_data_frame <- function() {
  return(SpatialPointsDataFrame(get_points(), get_expected_data_frame()))
}

get_expected_multi_points_data_frame <- function() {
  return(SpatialMultiPointsDataFrame(coords = get_multi_points(), data = get_expected_data_frame()))
}

get_expected_polygons_data_frame <- function() {
  return(SpatialPolygonsDataFrame(get_polygons(), data = get_expected_data_frame()))
}

get_expected_lines_data_frame <- function() {
  return(SpatialLinesDataFrame(get_lines(), data = get_expected_data_frame()))
}

get_expected_pixels_data_frame <- function() {
  return(SpatialPixelsDataFrame(get_points(), data = get_expected_data_frame()))
}

get_expected_grid_data_frame <- function() {
  return(SpatialGridDataFrame(get_grid(), data = get_expected_data_frame(), proj4string = CRS("epsg:4326")))
}

###############################################################################

test_that("is spatial data frame", {
  expect_false(.is_spatial_data_frame(get_data_frame()))
  expect_false(.is_spatial_data_frame(get_points()))
  expect_false(.is_spatial_data_frame(get_multi_points()))
  expect_false(.is_spatial_data_frame(get_polygons()))
  expect_false(.is_spatial_data_frame(get_lines()))
  expect_false(.is_spatial_data_frame(get_grid))

  expect_true(.is_spatial_data_frame(get_points_data_frame()))
  expect_true(.is_spatial_data_frame(get_multi_points_data_frame()))
  expect_true(.is_spatial_data_frame(get_polygons_data_frame()))
  expect_true(.is_spatial_data_frame(get_lines_data_frame()))
  expect_true(.is_spatial_data_frame(get_pixels_data_frame()))
  expect_true(.is_spatial_data_frame(get_grid_data_frame()))
})

###############################################################################

filter_numeric <- function(x) {
  return(Filter(is.numeric, x))
}

test_that("transform SpatialPointsDataFrame", {
  transformed <- .transform_data_frame(get_points_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_points_data_frame())
})

test_that("transform SpatialMultiPointsDataFrame", {
  transformed <- .transform_data_frame(get_multi_points_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_multi_points_data_frame())
})

test_that("transform SpatialPolygonsDataFrame", {
  transformed <- .transform_data_frame(get_polygons_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_polygons_data_frame())
})

test_that("transform SpatialLinesDataFrame", {
  transformed <- .transform_data_frame(get_lines_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_lines_data_frame())
})

test_that("transform SpatialPixelsDataFrame", {
  transformed <- .transform_data_frame(get_pixels_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_pixels_data_frame())
})

test_that("transform SpatialGridDataFrame", {
  transformed <- .transform_data_frame(get_grid_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_grid_data_frame())
})
