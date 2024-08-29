context("FisFusion")

test_that("new Fis fusion error", {
  skip_fusion_test()
  # granularity range error
  expect_error(NewFisFusion("foo", NULL, c(1, 2), NULL, NULL))
  expect_error(NewFisFusion("foo", NULL, c(2, 6), NULL, NULL))
  # conclusions length error
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c(0, 0, 0)))
  # conclusion numeric range error
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c(-1, 0, 0, 0)))
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c(0, 2, 0, 0)))
  # conclusion unique label error
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c("a", "a", "a", "a")))
  expect_error(NewFisFusion("foo", NULL, c(2, 3), NULL, c("a", "b", "c", "d", "e", "f")))
  # conclusion label error
  fl <- FusionLabel$new()
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c("a", "a", "b", "b"))) # 2 labels
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c("a", "a", "b", "c"))) # 3 labels
  expect_error(NewFisFusion("foo", NULL, c(2, 2), NULL, c("a", "b", "c", "d"))) # 4 labels
  expect_error(NewFisFusion("foo", NULL, c(2, 3), NULL, c("a", "a", "b", "c", "d", "e"))) # 5 labels
})

test_that("make input", {
  skip_fusion_test()
  expect_input(.MakeInput(2, "foo"), 2, "foo")
  expect_input(.MakeInput(3, "foo"), 3, "foo")
  expect_input(.MakeInput(4, "foo"), 4, "foo")
  expect_input(.MakeInput(5, "foo"), 5, "foo")
})

test_that("make crisp output", {
  skip_fusion_test()
  expect_crisp_output(.MakeCrispOutput("foo"), "foo")
})

test_that("make fuzzy output", {
  skip_fusion_test()
  expect_fuzzy_output(.MakeFuzzyOutput(2, "foo"), 2, "foo")
  expect_fuzzy_output(.MakeFuzzyOutput(3, "foo"), 3, "foo")
  expect_fuzzy_output(.MakeFuzzyOutput(4, "foo"), 4, "foo")
  expect_fuzzy_output(.MakeFuzzyOutput(5, "foo"), 5, "foo")
})

test_that("make premises", {
  skip_fusion_test()
  expected_premises <- matrix(c(
    1, 1,
    1, 2,
    2, 1,
    2, 2
  ), nrow = 4, ncol = 2, byrow = TRUE)
  expect_equal(.MakePremises(c(2, 2)), expected_premises)
  expected_premises <- matrix(c(
    1, 1, 1,
    1, 1, 2,
    1, 1, 3,
    1, 1, 4,
    1, 2, 1,
    1, 2, 2,
    1, 2, 3,
    1, 2, 4,
    1, 3, 1,
    1, 3, 2,
    1, 3, 3,
    1, 3, 4,
    2, 1, 1,
    2, 1, 2,
    2, 1, 3,
    2, 1, 4,
    2, 2, 1,
    2, 2, 2,
    2, 2, 3,
    2, 2, 4,
    2, 3, 1,
    2, 3, 2,
    2, 3, 3,
    2, 3, 4
  ), nrow = 24, ncol = 3, byrow = TRUE)
  expect_equal(.MakePremises(c(2, 3, 4)), expected_premises)
})

test_that("make crisp rules", {
  skip_fusion_test()
  rules <- .MakeRules(c(2, 2), c(0, 1, 2, 3))
  expect_rule(rules[[1]], c(1, 1), 0)
  expect_rule(rules[[2]], c(1, 2), 1)
  expect_rule(rules[[3]], c(2, 1), 2)
  expect_rule(rules[[4]], c(2, 2), 3)
  rules <- .MakeRules(c(2, 3), c(0, 1, 2, 3, 4, 5))
  expect_rule(rules[[1]], c(1, 1), 0)
  expect_rule(rules[[2]], c(1, 2), 1)
  expect_rule(rules[[3]], c(1, 3), 2)
  expect_rule(rules[[4]], c(2, 1), 3)
  expect_rule(rules[[5]], c(2, 2), 4)
  expect_rule(rules[[6]], c(2, 3), 5)
})

test_that("make fuzzy rules", {
  skip_fusion_test()
  fl <- FusionLabel$new()
  rules <- .MakeRules(c(2, 2), c(fl$high, fl$low, fl$low, fl$high))
  expect_rule(rules[[1]], c(1, 1), 2)
  expect_rule(rules[[2]], c(1, 2), 1)
  expect_rule(rules[[3]], c(2, 1), 1)
  expect_rule(rules[[4]], c(2, 2), 2)
  rules <- .MakeRules(c(2, 2), c(fl$average, fl$low, fl$low, fl$high))
  expect_rule(rules[[1]], c(1, 1), 2)
  expect_rule(rules[[2]], c(1, 2), 1)
  expect_rule(rules[[3]], c(2, 1), 1)
  expect_rule(rules[[4]], c(2, 2), 3)
  rules <- .MakeRules(c(2, 2), c(fl$very_high, fl$low, fl$high, fl$very_low))
  expect_rule(rules[[1]], c(1, 1), 4)
  expect_rule(rules[[2]], c(1, 2), 2)
  expect_rule(rules[[3]], c(2, 1), 3)
  expect_rule(rules[[4]], c(2, 2), 1)
  rules <- .MakeRules(c(2, 3), c(fl$average, fl$very_high, fl$low, fl$high, fl$very_low, fl$average))
  expect_rule(rules[[1]], c(1, 1), 3)
  expect_rule(rules[[2]], c(1, 2), 5)
  expect_rule(rules[[3]], c(1, 3), 2)
  expect_rule(rules[[4]], c(2, 1), 4)
  expect_rule(rules[[5]], c(2, 2), 1)
  expect_rule(rules[[6]], c(2, 3), 3)
})

test_that("new Fis fusion crisp output", {
  skip_fusion_test()
  fl <- FusionLabel$new()
  fis <- NewFisFusion("foo", c("input1", "input2"), c(2, 2), "output", c(0, 0.25, 0.75, 1))
  expect_equal(fis$name, "foo")
  expect_equal(fis$input_size(), 2)
  expect_input(fis$get_input(1), 2, "input1")
  expect_input(fis$get_input(2), 2, "input2")
  expect_equal(fis$output_size(), 1)
  expect_crisp_output(fis$get_output(1), "output")
  expect_equal(fis$rule_size(), 4)
  rules <- fis$get_rules()
  expect_rule(rules[[1]], c(1, 1), 0)
  expect_rule(rules[[2]], c(1, 2), 0.25)
  expect_rule(rules[[3]], c(2, 1), 0.75)
  expect_rule(rules[[4]], c(2, 2), 1)
})

test_that("new Fis fusion fuzzy output", {
  skip_fusion_test()
  fl <- FusionLabel$new()
  fis <- NewFisFusion("foo", c("input1", "input2"), c(2, 2), "output", c(fl$low, fl$low, fl$high, fl$high))
  expect_equal(fis$name, "foo")
  expect_equal(fis$input_size(), 2)
  expect_input(fis$get_input(1), 2, "input1")
  expect_input(fis$get_input(2), 2, "input2")
  expect_equal(fis$output_size(), 1)
  expect_fuzzy_output(fis$get_output(1), 2, "output")
  expect_equal(fis$rule_size(), 4)
  rules <- fis$get_rules()
  expect_rule(rules[[1]], c(1, 1), 1)
  expect_rule(rules[[2]], c(1, 2), 1)
  expect_rule(rules[[3]], c(2, 1), 2)
  expect_rule(rules[[4]], c(2, 2), 2)
})
