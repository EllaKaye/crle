#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

SEXP C_rle(SEXP x)
{
  // no need to call PROTECT(x), it is already in use
  if (!Rf_isReal(x)) Rf_error("`x` should be of the type 'double'");

  size_t n = XLENGTH(x);
  const double *xp = REAL(x);

  SEXP rle = PROTECT(Rf_allocVector(VECSXP, 2));

  SEXP values = PROTECT(Rf_allocVector(REALSXP, n));  // won't be GC'd
  double *valuesp = REAL(values);

  SEXP lengths = PROTECT(Rf_allocVector(INTSXP, n));  // won't be GC'd
  int *lengthsp = INTEGER(lengths);

  int index = 0;
  int rl = 1; // current run length

  for (size_t i = 0; i < n - 1; ++i) {
    // COULD DO: DEAL WITH NA

    // run continues
    if (xp[i] == xp[i + 1]) {
      rl++;
    } else { // run ends
      // update output vectors
      lengthsp[index] = rl;
      valuesp[index] = xp[i];

      //reset
      index++;
      rl = 1;
    }
  }

  // after loop, deal with final value
  if (xp[n - 2] == xp[n - 1]) {
    // have already added 1 to rl in last loop iteration
    // and index is in right place too
    lengthsp[index] = rl;
    valuesp[index] = xp[n - 1];
  } else {
    lengthsp[index] = 1;
    valuesp[index] = xp[n - 1];
  }

  // COULD DO: can return if index == n - 1

  // now restrict to length of rle
  SEXP values_rle = PROTECT(Rf_allocVector(REALSXP, index + 1));  // won't be GC'd
  double *values_rlep = REAL(values_rle);

  SEXP lengths_rle = PROTECT(Rf_allocVector(INTSXP, index + 1));  // won't be GC'd
  int *lengths_rlep = INTEGER(lengths_rle);

  for (size_t i = 0; i <= index; ++i) {
    values_rlep[i] = valuesp[i];
    lengths_rlep[i] = lengthsp[i];
  }

  SET_VECTOR_ELT(rle, 0, lengths_rle);
  SET_VECTOR_ELT(rle, 1, values_rle);

  UNPROTECT(5);  // pops five object from the protect stack;
  // does not trigger garbage collection, so we can return `y` now

  return rle;  // R will retrieve and protect it
}
