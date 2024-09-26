#include <stdio.h>
#include <math.h>
#include <fenv.h>

double factorial(double num) {
  double fac;

  if ((num == 0) || (num == 1))
    return 1;
  
  fac = 1;
  for (int i = 2; i <= num; i++)
    fac = fac * i;

  return fac;
}

/*double calculaPi(int iteracoes) {
  double pi_2, pi_2_ant;
  double fatorial_i, fatorial_2i;

  pi_2 = 0;
  pi_2_ant = 0;
  for (int i = 0; i <= iteracoes; i++) {
    pi_2_ant = pi_2;
    fatorial_i = factorial(i);
    fatorial_2i = factorial(2 * i + 1);
    pi_2 += ((pow(2, i) * fatorial_i * fatorial_i) / fatorial_2i);
    printf("aproximação atual - aproximação anterior = %.50lf\n", pi_2 - pi_2_ant);
  }

  printf("Com %d iterações, a aproximação para pi/2 foi (pi * 2): %.15e\n", iteracoes, pi_2 * 2);

  return pi_2;
}*/

double aproxPiMargin(double errorMargin, unsigned int *iterations) {
  double pi, prevPi, factorialAux1, factorialAux2;

  *iterations = 0;
  pi = 0;
  prevPi = 0;
  do {
    prevPi = pi;
    factorialAux1 = factorial(*iterations);
    factorialAux2 = factorial(2 * (*iterations) + 1);
    pi += ((pow(2, *iterations) * factorialAux1 * factorialAux1) / factorialAux2);
    (*iterations)++;
  } while ((pi - prevPi) > errorMargin);

  pi = pi * 2;
  return pi;
}

int main() {
  unsigned int iterations;
  double pi, errorMargin;

  fesetround(FE_UPWARD);
  //pi = calculaPi(iteracoes);

  printf("Digite a tolerância: ");
  scanf("%le", &errorMargin);

  pi = aproxPiMargin(errorMargin, &iterations);
  printf("%.15e\n", pi);
  printf("%d\n", iterations);

  return 0;
}