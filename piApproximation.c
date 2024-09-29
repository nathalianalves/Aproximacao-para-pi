#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <fenv.h>

#define _USE_MATH_DEFINES

// Retorna a diferença de ULPs entre uNum1 e uNum2
uint64_t ulpDifference(uint64_t uNum1, uint64_t uNum2) {
  if (uNum1 == uNum2)
    return 0;

  if (uNum1 < uNum2) 
    return uNum2 - uNum1;
  else 
    return uNum1 - uNum2;
}

// Retorna a fatorial de num
double factorial(double num, unsigned long int *floatingPointOperations) {
  double fac;

  if ((num == 0) || (num == 1))
    return 1;
  
  fac = 1;
  for (int i = 2; i <= num; i++) {
    fac = fac * i;
    (*floatingPointOperations)++;
  }

  return fac;
}

// Retorna a aproximação de pi com a margem de erro <= errorMargin
// *iterations armazena a quantidade de iterações feitas e *lastPi armazena a última aproximação calculada antes da retornada 
double aproxPi(double errorMargin, unsigned int *iterations, double *lastPi, unsigned long int *floatingPointOperations) {
  double pi, prevPi, factorialAux1, factorialAux2;
  
  *iterations = 0;
  pi = 0;
  prevPi = 0;
  do {
    prevPi = pi;
   
    factorialAux1 = factorial(*iterations, floatingPointOperations);
    factorialAux2 = factorial(2 * (*iterations) + 1, floatingPointOperations);
    pi += ((pow(2, *iterations) * factorialAux1 * factorialAux1) / factorialAux2) * 2;
    
    (*iterations)++;
    (*floatingPointOperations) += (3 + (*iterations)); // A conta em si faz 4 operações, e pow(x, y) faz cerca de y-1 multiplicações (4 + iterations - 1)
  } while ((pi - prevPi) > errorMargin);

  *lastPi = prevPi;

  return pi;
}

int main() {
  uint64_t ulpD, uPiUp, uPiDown, uApproxError, uExactError;
  unsigned int iterations;
  unsigned long int floatingPointOperationsDown, floatingPointOperationsUp;
  double errorMargin, lastPi, piUp, piDown, approximateError, exactError;

  floatingPointOperationsDown = 0;
  floatingPointOperationsUp = 0;

  printf("Digite a tolerância de erro desejada: ");
  scanf("%le", &errorMargin);

  // Calcula pi efetuando todos os arredondamentos para baixo
  fesetround(FE_DOWNWARD);
  piDown = aproxPi(errorMargin, &iterations, &lastPi, &floatingPointOperationsDown);

  // Calcula pi efetuando todos os arredondamentos para cima
  fesetround(FE_UPWARD);
  piUp = aproxPi(errorMargin, &iterations, &lastPi, &floatingPointOperationsUp);
  
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

  printf("\nIterações: %d\n", iterations); // Imprime a quantidade de iterações utilizada
  printf("Erro absoluto aproximado: %.15e %lX\n", approximateError, uApproxError); // Imprime o erro absoluto aproximado
  printf("Erro absoluto 'exato': %.15e %lX\n", exactError, uExactError); // Imprime o erro absoluto "exato"
  printf("Pi (baixo): %.15e %lX\n", piDown, uPiDown); // Imprime a aproximação de pi - arredondamentos para baixo
  printf("Pi (cima): %.15e %lX\n", piUp, uPiUp); // Imprime a aproximação de pi - arredondamentos para cima
  printf("Diferença de ULPs: %ld\n", ulpD); // Imprime a diferença de ULPs entre piUp e piDown
  printf("Número de operações de ponto-flutuante: %ld\n", floatingPointOperationsUp); // Imprime a quantidade de operações em variáveis double 

  return 0;
}