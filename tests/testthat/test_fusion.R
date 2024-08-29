context("Fusion")

test_that("new fusion aggreg error", {
  # WAM weights error
  expect_error(NewFusionAggreg("foo", NewAggregWam(c(0, 1)), Node$new("c1"), Node$new("c2"), Node$new("c3")))
  expect_no_error(NewFusionAggreg("foo", NewAggregWam(c(0, 1)), Node$new("c1"), Node$new("c2")))

  # OWA weights error
  expect_error(NewFusionAggreg("foo", NewAggregOwa(c(0, 1)), Node$new("c1"), Node$new("c2"), Node$new("c3")))
  expect_no_error(NewFusionAggreg("foo", NewAggregOwa(c(0, 1)), Node$new("c1"), Node$new("c2")))

  # Fis inputs error
  fis <- NewFis()
  fis$add_input(NewFisIn())
  fis$add_input(NewFisIn())
  fis$add_output(NewFisOutCrisp())
  expect_error(NewFusionAggreg("foo", NewAggregFis(fis), Node$new("c1"), Node$new("c2"), Node$new("c3")))
  expect_no_error(NewFusionAggreg("foo", NewAggregFis(fis), Node$new("c1"), Node$new("c2")))

  # no error with function aggreg
  expect_no_error(NewFusionAggreg("foo", NewAggregFunction(mean), Node$new("c1"), Node$new("c2")))
})
