expect_map_equal <- function(map, expected_map) {
  actual <- quasi_label(rlang::enquo(map))
  expect <- quasi_label(rlang::enquo(expected_map))
  expect_identical_crs(map, expected_map, label = actual$lab, expected.label = expect$lab)
  expect_equal(map, expected_map, label = actual$lab, expected.label = expect$lab, tolerance = 1e-3)
}

skip_zoning_test <- function() {
  if (identical(Sys.getenv("TEST_ZONING"), "true")) {
    return(invisible(TRUE))
  }
  skip("Skipping zoning test")
}
