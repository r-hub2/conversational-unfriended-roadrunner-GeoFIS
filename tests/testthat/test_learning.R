context("Learning")

test_that("Weights rounding", {
  weights <- c(0.10053848, 0.07200747, 0.18389546, 0.26936370, 0.30964647, 0.06454842)

  expect_equal(sum(weights), 1)

  expect_true(sum(round(weights, 6)) != 1)
  expect_equal(sum(.get_rounded_weights(weights, 6)), 1)

  expect_true(sum(round(weights, 5)) != 1)
  expect_equal(sum(.get_rounded_weights(weights, 5)), 1)

  expect_true(sum(round(weights, 4)) != 1)
  expect_equal(sum(.get_rounded_weights(weights, 4)), 1)

  expect_true(sum(round(weights, 3)) != 1)
  expect_equal(sum(.get_rounded_weights(weights, 3)), 1)

  expect_true(sum(round(weights, 2)) != 1)
  expect_equal(sum(.get_rounded_weights(weights, 2)), 1)
})

get_data <- function() {
  a <- c(.8, .7, .7, .6, .8, .2, .1, .3, .5, .8)
  b <- c(.9, .6, .7, .9, .6, .4, .2, .3, .2, .2)
  c <- c(.8, .9, .7, .9, .3, .2, .4, .3, .1, .2)
  d <- c(.1, .2, .2, .4, .9, .8, .1, .8, .2, .5)
  e <- c(.1, .3, .6, .4, .9, .1, .2, .3, .1, .1)
  data.frame(a, b, c, d, e)
}

target <- c(.7, .6, .6, .8, .8, .3, .1, .4, .3, .5)

test_that("WAM learning weights", {
  wam_weights <- LearnWamWeights(get_data(), target, 4)
  expected_weights <- c(0.4244, 0.4108, 0.0000, 0.1249, 0.0399)

  expect_equal(sum(wam_weights), 1)
  expect_equal(wam_weights, expected_weights)
})

test_that("OWA learning weights", {
  owa_weights <- LearnOwaWeights(get_data(), target, 4)
  # min to max OWA expected weights
  expected_weights <- c(0.1839, 0.0000, 0.0531, 0.6385, 0.1245)

  expect_equal(sum(owa_weights), 1)
  expect_equal(owa_weights, expected_weights)
})

test_that("Check range", {
  expected_error <- "data must be in range \\[0, 1\\]"

  expect_error(.check_range(c(0, NA, 1)), expected_error)
  expect_error(.check_range(c(-1, 0, 1)), expected_error)
  expect_error(.check_range(c(0, 1, 2)), expected_error)
  expect_no_error(.check_range(c(0, 0.5, 1)))

  expect_error(.check_range(matrix(c(0, NA, 1, 0), nrow = 2)), expected_error)
  expect_error(.check_range(matrix(c(-1, 0, 1, 0), nrow = 2)), expected_error)
  expect_error(.check_range(matrix(c(0, 1, 2, 0), nrow = 2)), expected_error)
  expect_no_error(.check_range(matrix(c(0, 0.5, 1, 0.5), nrow = 2)))

  expect_error(.check_range(data.frame(c(0, NA), c(1, 0))), expected_error)
  expect_error(.check_range(data.frame(c(-1, 0), c(1, 0))), expected_error)
  expect_error(.check_range(data.frame(c(0, 1), c(2, 0))), expected_error)
  expect_no_error(.check_range(data.frame(c(0, 1), c(0.5, 0))))
})
