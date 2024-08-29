context("AggregWam")

test_that("new aggreg WAM error", {
  # weights error
  expect_error(NewAggregWam(c(-1, 0)))
  expect_error(NewAggregWam(c(0.5, 0.6)))
})

test_that("compute WAM", {
  expect_equal(.ComputeWam(c(0, 1), c(1, 0)), 0)
  expect_equal(.ComputeWam(c(1, 0), c(1, 0)), 1)
  expect_equal(.ComputeWam(c(0, 1), c(0, 1)), 1)
  expect_equal(.ComputeWam(c(1, 0), c(0, 1)), 0)
  expect_equal(.ComputeWam(c(0, 1), c(0.25, 0.75)), 0.75)
  expect_equal(.ComputeWam(c(1, 0), c(0.25, 0.75)), 0.25)
  expect_equal(.ComputeWam(c(1, 1), c(0.25, 0.75)), 1)
})

test_that("test aggreg WAM", {
  leaf1 <- Node$new("l1")
  leaf1$sat <- 1
  leaf2 <- Node$new("l2")
  leaf2$sat <- 0
  aggreg_wam <- NewAggregWam(c(0, 1))
  node <- Node$new("ag")
  node$AddChildNode(leaf1)
  node$AddChildNode(leaf2)
  expect_equal(Aggreg(aggreg_wam, node, "sat"), 0)
})
