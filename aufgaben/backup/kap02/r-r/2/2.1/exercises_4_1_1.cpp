double ex_7(double x) {
  /* 4 1, 0 0 / 1 2, 4 0 */
  return (4 * x + 0) / (1 * x * x + 4 * 1 );
}


double ex_8(double x) {
  /* 4 1, 0 0 / 1 2, -4 0 */
  return (4 * x + 0) / (1 * x * x + -4 * 1 );
}


double ex_9(double x) {
  /* 1 2, -3 1, 1 0 / 1 2, 1 1, -6 0  */
  return (1 * x * x + -3 * x + 1 * 1 ) / (1 * x * x + 1 * x + -6 * 1 );
}


double ex_t1(double x) {
  /* 1 2, 0 1, 0 0 / 1 0  */
  return (1 * x * x + 0 + 0) / (1 * 1 );
}


double ex_t2(double x) {
  /* 1 2, 0 1, -5 0 / 1 0  */
  return (1 * x * x + 0 + -5 * 1 ) / (1 * 1 );
}


double ex_t3(double x) {
  /* 1 3, 2 2, 1 1, 0 0 / 1 2, -1 2, -2 1, 0 0 */
  return (1 * x * x * x + 2 * x * x + 1 * x + 0) / (1 * x * x + -1 * x * x + -2 * x + 0);
}


