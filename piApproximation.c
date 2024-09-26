#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <fenv.h>

#define _USE_MATH_DEFINES

// Retorna a fatorial de num
double factorial(double num) {
  double fac;

  if ((num == 0) || (num == 1))
    return 1;
  
  fac = 1;
  for (int i = 2; i <= num; i++)
    fac = fac * i;

  return fac;
}

// Retorna a diferença de ULPs entre num1 e num2
uint64_t ulpDifference(uint64_t uNum1, uint64_t uNum2) {
  if (uNum1 == uNum2)
    return 0;

  if (uNum1 < uNum2) 
    return uNum2 - uNum1;
  else 
    return uNum1 - uNum2;
}

// Retorna a aproximação de pi com a margem de erro <= errorMargin
// *iterations armazena a quantidade de iterações feitas e *lastPi armazena a última aproximação calculada antes da retornada 
double aproxPi(double errorMargin, unsigned int *iterations, double *lastPi) {
  double pi, prevPi, factorialAux1, factorialAux2;

  *iterations = 0;
  pi = 0;
  prevPi = 0;
  do {
    prevPi = pi;
    factorialAux1 = factorial(*iterations);
    factorialAux2 = factorial(2 * (*iterations) + 1);
    pi += ((pow(2, *iterations) * factorialAux1 * factorialAux1) / factorialAux2) * 2;
    (*iterations)++;
  } while ((pi - prevPi) > errorMargin);

  *lastPi = prevPi;
  pi = pi;

  return pi;
}

int main() {
  uint64_t ulpD, uPiUp, uPiDown, uApproxError, uExactError;
  unsigned int iterations;
  double errorMargin, lastPi, piUp, piDown, approximateError, exactError;

  scanf("%le", &errorMargin);

  // Calcula pi efetuando todos os arredondamentos para baixo
  fesetround(FE_DOWNWARD);
  piDown = aproxPi(errorMargin, &iterations, &lastPi);

  // Calcula pi efetuando todos os arredondamentos para cima
  fesetround(FE_UPWARD);
  piUp = aproxPi(errorMargin, &iterations, &lastPi);
  
  // Copia variáveis necessárias (piDown e piUp) para serem tratadas como inteiros de 64 bits
  memcpy(&uPiDown, &piDown, sizeof(double));
  memcpy(&uPiUp, &piUp, sizeof(double));

  // Configura variáveis auxiliares para impressão
  approximateError = fabs(piUp - lastPi);
  exactError = fabs(M_PI - piUp);
  ulpD = ulpDifference(uPiUp, uPiDown);

  // Copia variáveis necessárias (approximateError e exactError) para serem tratadas como inteiros de 64 bits
  memcpy(&uApproxError, &approximateError, sizeof(double));
  memcpy(&uExactError, &exactError, sizeof(double));

  printf("%d\n", iterations); // Imprime a quantidade de iterações utilizada
  printf("%.15e %lx\n", approximateError, uApproxError); // Imprime o erro absoluto aproximado
  printf("%.15e %lx\n", exactError, uExactError); // Imprime o erro absoluto "exato"
  printf("%.15e %lx\n", piDown, uPiDown); // Imprime a aproximação de pi - arredondamentos para baixo
  printf("%.15e %lx\n", piUp, uPiUp); // Imprime a aproximação de pi - arredondamentos para cima
  printf("%ld\n", ulpD); // Imprime a diferença de ULPs entre piUp e piDown
 
  return 0;
}