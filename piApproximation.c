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

// Faz o cálculo de (k!)^2/((2k + 1)!) de forma mais eficiente. "Divide" o problema em frações menores
double efficientCalc(double num, unsigned long int *floatingPointOperations) {
  double numAux, den, div, fac;
  int num1; // Usado para indicar se a primeira fatorial do numerador acabou. Quando esta acaba, se torna 1 e a segunda fatorial é calculada

  if (num == 0)
    return 1;
  
  if (num == 1)
    return ((double) 1/6);

  // Inicialização de variáveis utilizadas no loop principal
  num1 = 0;
  numAux = num;
  den = 2 * num + 1;
  div = 1;
  fac = 1;

  // Loop principal
  while (((!num1) || (numAux >= 1)) && (den >= 1)) {
    if (numAux != den) {
      div = numAux/den;
      fac = fac * div;
      (*floatingPointOperations) += 2;
    }
    
    den--;
    numAux--;
    if ((numAux == 0) && (!num1)) {
      num1 = 1;
      numAux = num;
    }
  }

  if (den > 1) {
    for (den = den - 1; den >= 1; den--) {
      div = 1/den;
      fac = fac * div;
    }
  } else if (numAux > 1) {
    for (numAux = numAux - 1; numAux >= 1; numAux--)
      fac = fac * num;
  }

  return fac;
}

// Retorna a aproximação de pi com a margem de erro <= errorMargin
// *iterations armazena a quantidade de iterações feitas e *lastPi armazena a última aproximação calculada antes da retornada 
double aproxPi(double errorMargin, unsigned int *iterations, double *lastPi, unsigned long int *floatingPointOperations) {
  double pi, prevPi, factorialAux;

  *iterations = 0;
  pi = 0;
  prevPi = 0;
  do {
    prevPi = pi;
    
    factorialAux = efficientCalc(*iterations, floatingPointOperations);
    pi += pow(2, *iterations) * factorialAux * 2;

    (*iterations)++;
    (*floatingPointOperations) += (1 + (*iterations)); // A conta em si faz 4 operações, e pow(x, y) faz cerca de y-1 multiplicações (4 + iterations - 1)
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