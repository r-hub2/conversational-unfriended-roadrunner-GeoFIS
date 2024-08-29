context("FusionLabel")

test_that("get fusion label", {
  fl <- FusionLabel$new()
  expect_equal(fl$get_labels(2), c(fl$low, fl$high))
  expect_equal(fl$get_labels(3), c(fl$low, fl$average, fl$high))
  expect_equal(fl$get_labels(4), c(fl$very_low, fl$low, fl$high, fl$very_high))
  expect_equal(fl$get_labels(5), c(fl$very_low, fl$low, fl$average, fl$high, fl$very_high))
})

test_that("get fusion label error", {
  fl <- FusionLabel$new()
  # granularity range error
  expect_error(fl$get_labels(1))
  expect_error(fl$get_labels(6))
})
