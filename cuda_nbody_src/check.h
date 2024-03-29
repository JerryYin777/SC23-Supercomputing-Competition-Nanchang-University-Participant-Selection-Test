#include <stdio.h>
#include <math.h>

void checkAccuracy(float *p, int nBodies)
{
  if (nBodies != 2<<11) return;

  int position1IsCorrect  = ( (int)(p[9*6]) ) == -11;
  int position2IsCorrect  = ( (int)(p[(9*6) + 1] ) == 3);
  int position3IsCorrect  = ( (int)(p[(9*6) + 2] ) == 10);
  int positionsAreCorrect = (position1IsCorrect == 1) && (position2IsCorrect == 1) && (position3IsCorrect == 1);

  printf("%s\n", positionsAreCorrect
      ? "Simulator is calculating positions correctly."
      : "Simulator is not calculating positions correctly.");

  return;
}