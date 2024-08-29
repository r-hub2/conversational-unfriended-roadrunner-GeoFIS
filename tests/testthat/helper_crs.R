#' see code of method 'identicalCRS1' in sp package CRS-methods.R
.identicalCRS1 <- function(x, y) {
  args_x <- strsplit(x@projargs, " +")[[1]]
  args_y <- strsplit(y@projargs, " +")[[1]]
  return(setequal(args_x, args_y))
}

expect_identical_crs <- function(object, expected, label = NULL, expected.label = NULL) {
  actual <- quasi_label(rlang::enquo(object), label)
  expect <- quasi_label(rlang::enquo(expected), expected.label)
  if (is(expected, "CRS")) {
    if (inherits(object, "ST")) {
      x <- slot(slot(object, "sp"), "proj4string")
    } else if (inherits(object, "Raster")) {
      x <- slot(object, "crs")
    } else {
      x <- slot(object, "proj4string")
    }
    expect(
      isTRUE(.identicalCRS1(x, expected)),
      sprintf("%s CRS not identical to %s", actual$lab, expect$lab)
    )
  } else {
    expect(
      isTRUE(identicalCRS(object, expected)),
      sprintf("%s CRS not identical to %s CRS", actual$lab, expect$lab)
    )
  }
}
