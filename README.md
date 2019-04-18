# Pipeline Gráfico

## 1. Introdução

Este trabalho tem como objetivo implementar o pipeline gráfico, passnado os vértices do objeto pelo espaço do objeto até o espaço de tela, através de transformações.

## 2. Transformações

Na primeira etapa do pipeline gráfico é realizada a transformação dos vértices do objeto que estão no espaço do objeto, local onde os objetos são criados, para o espaço do universo. Para isso, é criada a matriz model que pode conter a matriz identidade ou pode ser realizada uma composição de translações, escalas, rotações e shears, através da multiplicação dessas matrizes.

> ![alt-tex](Images/model_matrix.png)