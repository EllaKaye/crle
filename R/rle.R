#' Compute the Run Length Encoding
#'
#' Compute the run length encoding. This recreates the [rle()] function, but written in C.
#'
#' @param x a numeric vector
#'
#' @examples c_rle(c(1, 1, 3, 3, 3, 3, 2, 2, 2))
#'
#' @export
c_rle <- function(x) {
  if (!is.double(x)) x <- as.double(x)

  s <- .Call("C_rle", x)

  names(s) <- c("lengths", "values")
  class(s) <- "rle"

  s
}
