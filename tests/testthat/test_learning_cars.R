context("LearningCars")

test_that("Cars weights", {
  data(fusion_cars)

  a <- NewFusionInput("µA", NewMfTrapezoidalInf(4, 20), "A")
  v <- NewFusionInput("µV", NewMfTrapezoidalSup(100, 500), "V")
  s <- NewFusionInput("µS", NewMfTrapezoidalSup(120, 220), "S")
  c <- NewFusionInput("µC", NewMfTrapezoidalInf(6, 16), "C")

  degrees <- .aggregate_node(fusion_cars, list(a, v, s, c))
  target <- c(1, 0, 0, 0)

  wam_weights <- LearnWamWeights(degrees, target)
  expect_equal(wam_weights, c(0.5, 0, 0.5, 0))

  owa_weights <- LearnOwaWeights(degrees, target)
  expect_equal(owa_weights, c(1, 0, 0, 0))
})
