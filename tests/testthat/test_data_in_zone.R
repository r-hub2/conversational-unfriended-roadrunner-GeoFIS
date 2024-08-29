context("DataInZone")

expected_zones_map2 <- function(use_id) {
  if (!use_id) {
    return(as.integer(c(2, 2, 2, 2, 1, 2, 2, 2, 2)))
  } else {
    return(as.integer(c(1, 1, 1, 1, 5, 1, 1, 1, 1)))
  }
}

expected_zones_map4 <- function(use_id) {
  if (!use_id) {
    return(as.integer(c(3, 4, 4, 3, 1, 2, 3, 2, 2)))
  } else {
    return(as.integer(c(1, 2, 2, 1, 5, 6, 1, 6, 6)))
  }
}

expected_data_in_zone <- function(data, use_id) {
  map2 <- expected_zones_map2(use_id)
  map4 <- expected_zones_map4(use_id)
  return(cbind(data, data.frame(map2, map4)))
}

expected_data_in_zone_map2 <- function(data, use_id) {
  map2 <- expected_zones_map2(use_id)
  return(cbind(data, data.frame(map2)))
}

expect_zones_equal <- function(map, expected_map) {
  actual <- quasi_label(rlang::enquo(map))
  expect <- quasi_label(rlang::enquo(expected_map))
  expect_equal(map, expected_map, label = actual$lab, expected.label = expect$lab)
}

expect_data_in_zone_equal <- function(data_in_zone, expected_data_in_zone) {
  actual <- quasi_label(rlang::enquo(data_in_zone))
  expect <- quasi_label(rlang::enquo(expected_data_in_zone))
  expect_identical_crs(data_in_zone, expected_data_in_zone, label = actual$lab, expected.label = expect$lab)
  expect_equal(data_in_zone, expected_data_in_zone, label = actual$lab, expected.label = expect$lab, tolerance = 1e-3)
}

test_data_in_zone <- function(data, maps, use_id) {
  data_in_zone <- DataInZone(data, maps, use_id)
  expect_zones_equal(data_in_zone$map2, expected_zones_map2(use_id))
  expect_zones_equal(data_in_zone$map4, expected_zones_map4(use_id))
  expect_data_in_zone_equal(data_in_zone, expected_data_in_zone(data, use_id))
}

test_data_in_zone_map2 <- function(data, map2, use_id) {
  data_in_zone <- DataInZone(data, map2, use_id)
  expect_zones_equal(data_in_zone$map2, expected_zones_map2(use_id))
  expect_data_in_zone_equal(data_in_zone, expected_data_in_zone_map2(data, use_id))
}

###############################################################################

data_in_zone_crs <- CRS("epsg:2154")

test_that("data in zone", {
  skip_zoning_test()

  data <- get_source_3_3(data_in_zone_crs)
  border <- get_border_3_3(data_in_zone_crs)
  zoning <- NewZoning(data)
  zoning$border <- border
  zoning$perform_zoning()
  maps <- zoning$maps(c(2, 4))

  test_data_in_zone(data, maps, use_id = FALSE)
  test_data_in_zone(data, maps, use_id = TRUE)
})

test_that("data in zone map2", {
  skip_zoning_test()

  data <- get_source_3_3(data_in_zone_crs)
  border <- get_border_3_3(data_in_zone_crs)
  zoning <- NewZoning(data)
  zoning$border <- border
  zoning$perform_zoning()
  map2 <- zoning$map(2)

  test_data_in_zone_map2(data, map2, use_id = FALSE)
  test_data_in_zone_map2(data, map2, use_id = TRUE)
})
