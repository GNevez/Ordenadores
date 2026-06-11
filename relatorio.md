# Relatorio: tempo de execucao de algoritmos de ordenacao

## 1. Objetivo

Esta atividade compara o tempo de execucao dos algoritmos Bubble Sort,
Merge Sort e Quick Sort. O experimento busca demonstrar que o desempenho
depende do tamanho e da disposicao dos dados de entrada, alem da
complexidade assintotica de cada algoritmo.

## 2. Algoritmos e complexidades

| Algoritmo | Melhor caso | Caso medio | Pior caso |
|---|---:|---:|---:|
| Bubble Sort otimizado | O(n) | O(n^2) | O(n^2) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) |
| Quick Sort | O(n log n) | O(n log n) | O(n^2) |

O Quick Sort utilizado escolhe o elemento central como pivo. Seu pior caso
continua sendo quadratico, embora em entradas aleatorias o comportamento
medio esperado seja O(n log n).

## 3. Funcao de medida de tempo

A funcao disponibilizada no AVA utiliza `clock()` para obter o instante
inicial e final. O tempo em segundos e calculado por:

```c
tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
```

Para tornar o resultado mais representativo para uma pessoa, o programa
converteu o valor para milissegundos:

```c
tempo_ms = tempo_cpu * 1000.0;
```

Ordenacoes muito pequenas podem terminar antes da resolucao do relogio e
produzir zero. Por isso, cada teste foi repetido ate acumular pelo menos
0,10 segundo. O resultado apresentado e a media por execucao. O custo de
copiar o vetor foi medido separadamente e descontado.

## 4. Teste com o mesmo vetor

Os tres algoritmos foram executados sobre copias do vetor:

```text
[42, 7, 19, 3, 88, 51, 14, 1, 73, 26]
```

| Algoritmo | Tempo medio |
|---|---:|
| Bubble Sort | 0,000027 ms |
| Merge Sort | 0,000122 ms |
| Quick Sort | 0,000030 ms |

Nesse vetor pequeno, as diferencas sao muito reduzidas. O Merge Sort ainda
precisa criar um vetor auxiliar e executar chamadas recursivas, fazendo com
que seu custo fixo tenha maior influencia.

## 5. Testes com vetores randomizados

Foram criados vetores com 100, 300, 500, 1.000 e 10.000 elementos. A
semente `2026` foi usada para permitir que o experimento seja reproduzido.
Em cada tamanho, todos os algoritmos receberam uma copia do mesmo vetor.

| Elementos | Bubble Sort (ms) | Merge Sort (ms) | Quick Sort (ms) |
|---:|---:|---:|---:|
| 100 | 0,011047 | 0,002075 | 0,001526 |
| 300 | 0,094727 | 0,006775 | 0,005219 |
| 500 | 0,271484 | 0,011963 | 0,009155 |
| 1.000 | 1,070313 | 0,026855 | 0,020020 |
| 10.000 | 104,000000 | 0,671875 | 0,601563 |

Os tempos foram obtidos nesta maquina e podem variar conforme processador,
sistema operacional, compilador, otimizacoes e processos em segundo plano.

## 6. Grafico

![Grafico dos tempos](grafico_tempos.svg)

O primeiro painel apresenta os tres algoritmos em linhas e na mesma escala
linear, de 0 a 120 ms. O segundo painel amplia somente os resultados de
Merge Sort e Quick Sort para 100, 300 e 500 elementos, usando uma escala
linear de 0 a 0,015 ms. Os valores exatos foram escritos nos pontos do
painel ampliado para permitir a comparacao direta com a tabela.

## 7. Analise dos resultados

De 1.000 para 10.000 elementos, a entrada aumentou dez vezes. O tempo do
Bubble Sort passou de aproximadamente 1,07 ms para 104 ms, um aumento de
aproximadamente 97 vezes. Esse crescimento e coerente com sua complexidade
O(n^2).

Merge Sort e Quick Sort cresceram de forma bem menos acentuada, conforme o
comportamento O(n log n) esperado para essas entradas. Com 10.000
elementos, o Quick Sort foi o mais rapido, seguido de perto pelo Merge Sort.
O Bubble Sort demorou aproximadamente 173 vezes mais que o Quick Sort.

## 8. Conclusao

O experimento confirmou que a escolha do algoritmo se torna mais importante
conforme a entrada cresce. Em vetores pequenos, custos fixos e a resolucao
do relogio podem esconder as diferencas. Em vetores maiores, a complexidade
quadratica do Bubble Sort produz um crescimento muito superior ao dos
algoritmos O(n log n).

Portanto, Merge Sort e Quick Sort sao escolhas mais adequadas para grandes
volumes de dados. A entrada tambem influencia o resultado: o Quick Sort
pode chegar a O(n^2) dependendo da escolha dos pivos, enquanto o Merge Sort
mantem O(n log n) em todos os casos.
