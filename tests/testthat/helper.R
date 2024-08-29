expect_no_error <- function(expr) {
  err <- FALSE
  message <- NULL
  tryCatch(force(expr), error = function(e) {
    err <<- TRUE
    message <<- e
  })
  expect(!err, paste("Expected no error, but had error:", message))
  invisible(NULL)
}
