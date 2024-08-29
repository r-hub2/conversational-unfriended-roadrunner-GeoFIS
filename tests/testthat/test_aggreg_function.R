context("AggregFunction")

test_that("test aggreg function", {
  leaf1 <- Node$new("l1")
  leaf1$sat <- 0.25
  leaf2 <- Node$new("l2")
  leaf2$sat <- 0.75
  aggreg_function <- NewAggregFunction(mean)
  node <- Node$new("ag")
  node$AddChildNode(leaf1)
  node$AddChildNode(leaf2)
  expect_equal(Aggreg(aggreg_function, node, "sat"), 0.5)
})
