context("AggregFis")

test_that("test aggreg Fis error", {
  skip_fusion_test()
  input1 <- NewFisIn()
  input1$name <- "foo1"
  input2 <- NewFisIn(0, 2)
  input2$name <- "foo2"
  fis <- NewFis()
  fis$add_input(input1)
  fis$add_input(input2)
  expect_error(NewAggregFis(fis)) # input2 range error

  fis <- NewFis()
  fis$add_input(NewFisIn())
  fis$add_input(NewFisIn())
  expect_error(NewAggregFis(fis)) # output_index error

  output <- NewFisOutCrisp(0, 2)
  output$name <- "bar"
  fis$add_output(output)
  expect_error(NewAggregFis(fis)) # output range error

  fis <- NewFis()
  fis$add_input(NewFisIn(2, 0, 1))
  fis$add_input(NewFisIn(2, 0, 1))
  fis$add_output(NewFisOutCrisp(0, 1))
  fis$add_output(NewFisOutFuzzy(2, 0, 1))
  suppressWarnings(fis$add_rule(NewRule(c(1, 1), c(2, 1))))
  expect_error(NewAggregFis(fis)) # output crisp conclusion range error

  fis <- NewFis()
  fis$add_input(NewFisIn(2, 0, 1))
  fis$add_input(NewFisIn(2, 0, 1))
  fis$add_output(NewFisOutCrisp(0, 1))
  fis$add_output(NewFisOutFuzzy(2, 0, 1))
  fis$add_rule(NewRule(c(1, 1), c(1, 1)))
  expect_no_error(NewAggregFis(fis))
})

test_that("test aggreg Fis", {
  skip_fusion_test()
  fis <- NewFis("test.fis")
  leaf1 <- Node$new("l1")
  leaf1$sat <- 0.25
  leaf2 <- Node$new("l2")
  leaf2$sat <- 0.75
  aggreg_fis <- NewAggregFis(fis)
  node <- Node$new("ag")
  node$AddChildNode(leaf1)
  node$AddChildNode(leaf2)
  expect_equal(Aggreg(aggreg_fis, node, "sat"), 0.333, tolerance = 1e-3)
  aggreg_fis <- NewAggregFis(fis, 2)
  expect_equal(Aggreg(aggreg_fis, node, "sat"), 0.361, tolerance = 1e-3)
})
