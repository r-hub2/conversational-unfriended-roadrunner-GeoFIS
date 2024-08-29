skip_fusion_test <- function() {
  if (identical(Sys.getenv("TEST_FUSION"), "true")) {
    return(invisible(TRUE))
  }
  skip("Skipping fusion test")
}
