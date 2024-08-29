context("AggregateNode")

library(sf)

leaf1 <- NewFusionInput("l1", NewMfTrapezoidalInf(0, 4), "a1")
leaf2 <- NewFusionInput("l2", NewMfTrapezoidalSup(0, 4), "a1")
leaf3 <- NewFusionInput("l3", NewMfTrapezoidal(0, 4, 4, 8), "a2")
leaf4 <- NewFusionInput("l4", NewMfTriangular(0, 4, 8), "a2")

aggregate1 <- NewFusionAggreg("ag1", NewAggregOwa(c(1, 0)), leaf1, leaf3)
aggregate2 <- NewFusionAggreg("ag2", NewAggregWam(c(1, 0)), leaf2, leaf4)
aggregate3 <- NewFusionAggreg("ag3", NewAggregWam(c(0.5, 0.5)), aggregate1, aggregate2)

aggregate_crs <- CRS("epsg:4326")

get_points <- function() {
  return(get_spatial_points(
    "GEOMETRYCOLLECTION(
    POINT(0 0), POINT(0 1),
    POINT(1 0), POINT(1 1),
    POINT(2 0), POINT(2 1),
    POINT(3 0), POINT(3 1))",
    aggregate_crs
  ))
}

get_multi_points <- function() {
  points1 <- get_spatial("MULTIPOINT((0 0), (0 1))", aggregate_crs)
  points2 <- get_spatial("MULTIPOINT((1 0), (1 1))", aggregate_crs)
  points3 <- get_spatial("MULTIPOINT((2 0), (2 1))", aggregate_crs)
  points4 <- get_spatial("MULTIPOINT((3 0), (3 1))", aggregate_crs)
  points5 <- get_spatial("MULTIPOINT((4 0), (4 1))", aggregate_crs)
  points6 <- get_spatial("MULTIPOINT((5 0), (5 1))", aggregate_crs)
  points7 <- get_spatial("MULTIPOINT((6 0), (6 1))", aggregate_crs)
  points8 <- get_spatial("MULTIPOINT((7 0), (7 1))", aggregate_crs)
  return(SpatialMultiPoints(coords = list(points1, points2, points3, points4, points5, points6, points7, points8), proj4string = rebuild_CRS(points1@proj4string)))
}

get_polygons <- function() {
  return(get_spatial_polygons(
    "GEOMETRYCOLLECTION(
    POLYGON((0 0, 0 1, 1 1, 1 0, 0 0)),
    POLYGON((1 0, 1 1, 2 1, 2 0, 1 0)),
    POLYGON((2 0, 2 1, 3 1, 3 0, 2 0)),
    POLYGON((3 0, 3 1, 4 1, 4 0, 3 0)),
    POLYGON((4 0, 4 1, 5 1, 5 0, 4 0)),
    POLYGON((5 0, 5 1, 6 1, 6 0, 5 0)),
    POLYGON((6 0, 6 1, 7 1, 7 0, 6 0)),
    POLYGON((7 0, 7 1, 8 1, 8 0, 7 0)))",
    aggregate_crs
  ))
}

get_lines <- function() {
  return(get_spatial_lines(
    "GEOMETRYCOLLECTION(
    LINESTRING(0 0, 0 1),
    LINESTRING(1 0, 1 1),
    LINESTRING(2 0, 2 1),
    LINESTRING(3 0, 3 1),
    LINESTRING(4 0, 4 1),
    LINESTRING(5 0, 5 1),
    LINESTRING(6 0, 6 1),
    LINESTRING(7 0, 7 1))",
    aggregate_crs
  ))
}

get_grid <- function() {
  return(GridTopology(cellcentre.offset = c(0, 0), cellsize = c(1, 1), cells.dim = c(4, 2)))
}

get_data_frame_source <- function() {
  a1 <- c(0, 1, 2, 3, 4, 5, 6, 7)
  a2 <- c(8, 7, 6, 5, 4, 3, 2, 1)
  return(data.frame(a1, a2))
}

get_points_source <- function() {
  return(SpatialPointsDataFrame(get_points(), get_data_frame_source()))
}

get_multi_points_source <- function() {
  return(SpatialMultiPointsDataFrame(coords = get_multi_points(), data = get_data_frame_source()))
}

get_polygons_source <- function() {
  return(SpatialPolygonsDataFrame(get_polygons(), data = get_data_frame_source()))
}

get_lines_source <- function() {
  return(SpatialLinesDataFrame(get_lines(), data = get_data_frame_source()))
}

get_pixels_source <- function() {
  return(SpatialPixelsDataFrame(get_points(), data = get_data_frame_source()))
}

get_grid_source <- function() {
  return(SpatialGridDataFrame(get_grid(), data = get_data_frame_source(), proj4string = CRS("epsg:4326")))
}

###############################################################################

test_points <- function(result) {
  expected_points <- get_points()
  expect_identical(result@coords, expected_points@coords)
}

test_multi_points <- function(result) {
  expected_multi_points <- get_multi_points()
  expect_identical(result@coords, expected_multi_points@coords)
}

test_polygons <- function(result) {
  expected_polygons <- get_polygons()
  expect_identical(result@polygons, expected_polygons@polygons)
}

test_lines <- function(result) {
  expected_lines <- get_lines()
  expect_identical(result@lines, expected_lines@lines)
}

test_grid <- function(result) {
  expected_grid <- get_grid()
  expect_identical(result@grid, expected_grid)
}

test_aggregate_leaf1 <- function(result) {
  expect_equal(result[["l1"]], c(1, 0.75, 0.5, 0.25, 0, 0, 0, 0))
}

setGeneric(name = "test_aggregate_node1", def = function(result) {
  standardGeneric("test_aggregate_node1")
})

setMethod(f = "test_aggregate_node1", signature = "data.frame", definition = function(result) {
  test_aggregate_leaf1(result)
  expect_equal(result[["l3"]], c(0, 0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25))
  expect_equal(result[["ag1"]], c(0, 0.25, 0.5, 0.25, 0, 0, 0, 0))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialPointsDataFrame", definition = function(result) {
  test_points(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialMultiPointsDataFrame", definition = function(result) {
  test_multi_points(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialPolygonsDataFrame", definition = function(result) {
  test_polygons(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialLinesDataFrame", definition = function(result) {
  test_lines(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialPixelsDataFrame", definition = function(result) {
  test_points(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_node1", signature = "SpatialGridDataFrame", definition = function(result) {
  test_grid(result)
  test_aggregate_node1(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

test_aggregate_node2 <- function(result) {
  expect_equal(result[["l2"]], c(0, 0.25, 0.5, 0.75, 1, 1, 1, 1))
  expect_equal(result[["l4"]], c(0, 0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25))
  expect_equal(result[["ag2"]], c(0, 0.25, 0.5, 0.75, 1, 1, 1, 1))
}

test_aggregate_node3 <- function(result) {
  test_aggregate_node1(result)
  test_aggregate_node2(result)
  expect_equal(result[["ag3"]], c(0, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5))
}

setGeneric(name = "test_aggregate_nodes", def = function(result) {
  standardGeneric("test_aggregate_nodes")
})

setMethod(f = "test_aggregate_nodes", signature = "data.frame", definition = function(result) {
  test_aggregate_node1(result)
  test_aggregate_node2(result)
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialPointsDataFrame", definition = function(result) {
  test_points(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialMultiPointsDataFrame", definition = function(result) {
  test_multi_points(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialPolygonsDataFrame", definition = function(result) {
  test_polygons(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialLinesDataFrame", definition = function(result) {
  test_lines(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialPixelsDataFrame", definition = function(result) {
  test_points(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

setMethod(f = "test_aggregate_nodes", signature = "SpatialGridDataFrame", definition = function(result) {
  test_grid(result)
  test_aggregate_nodes(result@data)
  expect_identical_crs(result, CRS("epsg:4326"))
})

###############################################################################

test_that("leaf aggregate", {
  test_aggregate_leaf1(.aggregate_node(get_data_frame_source(), leaf1))
})

test_that("data frame aggregate", {
  test_aggregate_node1(.aggregate_node(get_data_frame_source(), aggregate1))
  test_aggregate_node2(.aggregate_node(get_data_frame_source(), aggregate2))
  test_aggregate_node3(.aggregate_node(get_data_frame_source(), aggregate3))
  test_aggregate_nodes(.aggregate_node(get_data_frame_source(), list(aggregate1, aggregate2)))
})

test_that("spatial points aggregate", {
  test_aggregate_node1(.aggregate_node(get_points_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_points_source(), list(aggregate1, aggregate2)))
})

test_that("spatial multi points aggregate", {
  test_aggregate_node1(.aggregate_node(get_multi_points_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_multi_points_source(), list(aggregate1, aggregate2)))
})

test_that("spatial polygons aggregate", {
  test_aggregate_node1(.aggregate_node(get_polygons_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_polygons_source(), list(aggregate1, aggregate2)))
})

test_that("spatial lines aggregate", {
  test_aggregate_node1(.aggregate_node(get_lines_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_lines_source(), list(aggregate1, aggregate2)))
})

test_that("spatial pixels aggregate", {
  test_aggregate_node1(.aggregate_node(get_pixels_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_pixels_source(), list(aggregate1, aggregate2)))
})

test_that("spatial grid aggregate", {
  test_aggregate_node1(.aggregate_node(get_grid_source(), aggregate1))
  test_aggregate_nodes(.aggregate_node(get_grid_source(), list(aggregate1, aggregate2)))
})
