# OpenGL

## Olá, triângulo

Tudo em OpenGL é 3D, mas a janela é apenas um array de pixels 2D. Muito do que
o OpenGL fará é transformar as coordenadas 3D em pixels 2D que possam ser
renderizadas na tela. Tudo isso é feito pela pipeline gráfica do OpenGL.

Esse processo de transformação pode ser resumido em duas grandes etapas:
Coordenadas 3D para pixels 2D -> Pixels 2D para pixels coloridos

O pipeline gráfico do OpenGL recebera uma lista de vértices como input. Pense
em um vértice com uma tupla de três elementos os quais representam uma
coordenada 3D (x, y, z).

OpenGL apenas transforma coordenadas 3D que estão dentro de um intervalo, este
seria o -1 e 1 em todos os três eixos (x, y e z), se estiver nesse intervalo,
os dados serão visíveis na tela, caso contrário não veremos na tela.
