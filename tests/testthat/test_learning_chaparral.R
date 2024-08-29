context("LearningChaparral")

chaparral <- read.table("chaparral_data.csv", sep = ",", header = TRUE)
chaparral_data <- chaparral[, 1:3]
chaparral_target <- chaparral[, 4]

test_that("Chaparral WAM learning weights", {
  wam_weights <- LearnWamWeights(chaparral_data, chaparral_target)
  # see WAM expected weights in [@denys20]
  expected_weights <- c(0.317, 0.312, 0.371)

  expect_equal(sum(wam_weights), 1)
  expect_equal(wam_weights, expected_weights)
})

test_that("Chaparral OWA learning weights", {
  owa_weights <- LearnOwaWeights(chaparral_data, chaparral_target)
  # see min to max OWA expected weights in [@denys20]
  expected_weights <- c(0.407, 0.220, 0.373)

  expect_equal(sum(owa_weights), 1)
  expect_equal(owa_weights, expected_weights)
})
