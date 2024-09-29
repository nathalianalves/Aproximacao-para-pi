O objetivo do projeto é aproximar pi pelo somatório:
<div align="center">
<img src="/assets/somatorio.png">
</div>

O cálculo para quando o erro absoluto aproximado (diferença entre a aproximação de pi atual e a anterior) é, no máximo, a tolerância digitada pelo usuário.

Nenhuma otimização foi feita sobre as operações executadas. 
  - pi(baixo): aproximação de pi com todos os cálculos sendo arredondados para baixo
  - pi(cima): aproximação de pi com todos os cálculos sendo arredondados para cima

Descrição das saídas:
  - iterações: número de vezes que o somátorio foi executado
  - erro absoluto aproximado: diferença entre a aproximação de pi apresentada na saída e a aproximação anterior
  - erro absoluto "exato": diferença entre a aproximação de pi apresentada na saída e o valor da constante M_PI (em math.h)
  - pi(baixo): aproximação de pi com arredondamento para baixo
  - pi(cima): aproximação de pi com arredondamento para cima
  - diferença de ULPs: diferença de ULPs entre as duas aproximações de pi apresentadas
  - quantidade de operações de ponto-flutuante: número de operações feitas sobre variáveis do tipo double 


Observações:
  - Os erros e número de operações de ponto-flutuante apresentados são baseados no cálculo de pi(cima)
  - Algumas saídas são impressas, também, em hexadecimal

A compilação pode ser feita pelo comando make no terminal. 
Nome do executável: pi
