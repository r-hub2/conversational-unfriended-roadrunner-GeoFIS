expect_name <- function(object, expected_name, label = NULL) {
  actual <- quasi_label(rlang::enquo(object), label)
  expect(
    object$name == expected_name,
    sprintf("%s is not named %s", actual$lab, expected_name)
  )
}

expect_range <- function(object, label = NULL) {
  actual <- quasi_label(rlang::enquo(object), label)
  expected_range <- c("min" = 0, "max" = 1)
  expect(
    isTRUE(all.equal(object$range(), expected_range)),
    sprintf("%s has not a range [0, 1]", actual$lab)
  )
}

expect_granularity <- function(object, expected_granularity, label = NULL) {
  actual <- quasi_label(rlang::enquo(object), label)
  expect(
    object$mf_size() == expected_granularity,
    sprintf("%s has not a granularity %i", actual$lab, expected_granularity)
  )
}

expect_labels <- function(object, granularity, label = NULL) {
  actual <- quasi_label(rlang::enquo(object), label)
  fl <- FusionLabel$new()
  expected_labels <- fl$get_labels(granularity)
  labels <- sapply(object$get_mfs(), FUN = function(mf) mf$label)
  expect(
    isTRUE(all.equal(labels, expected_labels)),
    sprintf("%s is not an input with labels [%s]", actual$lab, paste0(expected_labels, collapse = ", "))
  )
}

expect_input <- function(input, expected_granularity, expected_name) {
  actual <- quasi_label(rlang::enquo(input))
  expect_name(input, expected_name, actual$lab)
  expect_range(input, actual$lab)
  expect_granularity(input, expected_granularity, actual$lab)
  expect_labels(input, expected_granularity, actual$lab)
}

expect_crisp_output <- function(output, expected_name) {
  actual <- quasi_label(rlang::enquo(output))
  expect_is(output, FisOutCrisp)
  expect_name(output, expected_name, actual$lab)
  expect_range(output, actual$lab)
}

expect_fuzzy_output <- function(output, expected_granularity, expected_name) {
  actual <- quasi_label(rlang::enquo(output))
  expect_is(output, FisOutFuzzy)
  expect_name(output, expected_name, actual$lab)
  expect_range(output, actual$lab)
  expect_granularity(output, expected_granularity, actual$lab)
  expect_labels(output, expected_granularity, actual$lab)
}

expect_rule <- function(rule, expected_premises, expected_conclusions, label = NULL) {
  actual <- quasi_label(rlang::enquo(rule), label)
  expect_equal(rule$premises, expected_premises, label = actual$lab)
  expect_equal(rule$conclusions, expected_conclusions, label = actual$lab)
}
