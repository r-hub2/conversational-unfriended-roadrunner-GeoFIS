context("DataFrame")

get_data_frame <- function() {
  a <- as.integer(c(0, 1, 2, 3))
  b <- c("a", "b", "c", "d")
  c <- c(4, 5, 6, 7)
  return(data.frame(a, b, c))
}

get_expected_data_frame <- function() {
  a <- as.integer(c(0, 1, 2, 3))
  c <- c(4, 5, 6, 7)
  return(data.frame(a, c))
}

get_expected_empty_data_frame <- function() {
  return(data.frame(matrix(NA, nrow = 4, ncol = 0)))
}

###############################################################################

filter_numeric <- function(x) {
  return(Filter(is.numeric, x))
}

test_that("transform data.frame", {
  transformed <- .transform_data_frame(get_data_frame(), filter_numeric)
  expect_equal(transformed, get_expected_data_frame())
})

filter_all <- function(x) {
  return(FALSE)
}

no_filter <- function(x) {
  return(TRUE)
}

test_that("filter data.frame", {
  # filter numeric
  filtered <- .filter_data_frame(get_data_frame(), is.numeric)
  expect_equal(filtered, get_expected_data_frame())

  # no filter
  filtered <- .filter_data_frame(get_data_frame(), no_filter)
  expect_equal(filtered, get_data_frame())

  # filter all
  filtered <- .filter_data_frame(get_data_frame(), filter_all)
  expect_equal(filtered, get_expected_empty_data_frame())
})
