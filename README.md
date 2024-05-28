# OpenGL

OpenGL é uma API de gráficos que nos permite interagir com nossa GPU (Graphic
Processing Unit). Por baixo de tudo, OpenGL não é nem mesmo uma biblioteca, mas
sim apenas uma especificação, pois o que vemos é uma lista de métodos com seus
nomes, parâmetros, tipos de retornos e é isso. O "código de implementação do
OpenGL" é feito pelo produtor da GPU que você está usando, então você não
necessariamente faz o download do OpenGL porque empresas como AMD, Intel,
Nvidia possuem suas próprias implementações de OpenGL.

Existe uma diferença entre Legacy e Modern OpenGL. Antigamente nós não tínhamos
muito controle sobre a GPU, então as instruções eram bem mais simples para
poder desenhar os gráficos, hoje em dia temos muito mais controle e poder.
Basicamente todas as notas abaixo estarão relacionadas ao Modern OpenGL.

Um exemplo do quão diferente Legacy e Modern OpenGL são. No Legacy, você não
precisa saber nada a respeito de shaders e vertex buffers para poder ter
triângulo na tela. Veja o arquivo
[legacy_opengl_triangle.cpp](./legacy_opengl_triangle.cpp), mais
especificamente no while loop onde temos instruções como glBegin(GL_TRIANGLES)
e depois instruções de onde ficam cada vértice, isso é Legacy OpenGL.

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

Antes de passar para o Modern OpenGL, gostaria de deixar claro que um vértice
é muito mais do que apenas a posição (coordenada). Um vértice contém a posição,
mas possui muito mais do que isso, ele contém cores e dentre outros atributos
que compõem o que um vértice é.

Em Modern OpenGL, temos alguns conceitos importantes para
poder começar a entender: Vertex buffer e shaders.

Vertex buffers são apenas buffers, ou seja, um array de bytes. Esse array de
buffers fica armazenado em nossa GPU. Nós armazenamos na GPU para que possamos
ler depois e desenhar na tela. Nosso trabalho será armazenar na GPU e instruir
como armazenar aqueles dados e como renderizar na tela para a GPU, essa última
etapa nós instruímos através de um programa chamado "shader", um shader é um
programa que roda na GPU que serve para dizer como iremos renderizar na tela o
vertex buffer que temos armazenado na memória da GPU.

No final, OpenGL é uma state machine: Selecione o buffer com id x, selecione o
shader y e desenhe um triângulo. E isso se repete várias vezes: buffer ->
shader -> draw. Antes em Legacy OpenGL, não precisávamos ter essa sequência,
nós apenas especificamos os vértices diretamente e acabou. Em Modern OpenGL,
temos mais controle sobre as informações que estamos operando, podemos
armazenar buffers diretamente na GPU, acessar aqueles dados, ler para
renderizar, alterar os dados para depois renderizar e etc.

Agora é interessante notar uma coisa: shader é um programa que é executado na
GPU para que possamos dizer como os dados devem ser renderizados. Nós usamos
instruções do OpenGL para poder armazenar informações na GPU de forma que o
programa de shaders tenha acesso lá na GPU. Por isso que é importante que os
dados já estejam na GPU antes de você chamar os shaders para poder armazenar do
contrário, não teremos nada para renderizar.

Outro conceito importante de OpenGL é o layout. Quando passamos as informações
para a GPU, nós passamos como um void*, apenas um ponteiro para bytes na
memória, void* não nos diz absolutamente nada a respeito dos dados que estamos
lidando, só sabemos que é um ponteiro para bytes na memória ou para um endereço
NULL. Sendo assim, precisamos de alguma forma instruir a GPU a respeito do
layout daqueles dados para que possamos interpretar as informaçõe de forma
apropriada e renderizar nossos dados da forma que esperamos. Sem esse contexto,
a GPU não consegue adivinhar o layout dos dados que estamos passando para ela,
você pode pensar que o layout é óbvio, mas isso para nós seres humanos que temos
o contexto, mas computadores não enxergam padrões como a gente, apenas uma série
de bytes armazenados na memória, por isso que precisamos contextualizar a GPU.

Falando sobre shaders, na maior parte do tempo iremos lidar com dois tipos
de shaders: vertex e fragment shaders (pixel shaders).

Falando sobre o vertex shader, o vertex shader é um programa que será chamado N
vezes, onde N é a quantidade de vértices que temos. Basicamente, o vertex
shader será usado para indicar a posição do vértice na tela. Depois disso,
temos o fragment shader, que será usado para decidir qual cor o determinado
pixel / fragmento terá na tela. O fragment shader irá rodar para cada pixel,
então precisa ter cuidado para não colocar operações super pesadas no fragment
shader.

Uma coisa interessante a se notar é que quando estamos criando quadrados, por
exemplo, nós construímos dois triângulos. Esses dois triângulos se conectam
para formar um quadrado. Se não tivermos cuidado, nós iremos repetir vértices e
isso pode consumir memória de forma desnecessária. Em razão disso, podemos usar
index buffers para reaproveitar vértices e usar uma quantidade menor de recursos
computacionais.
