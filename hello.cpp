// clang-format off
// It is important that <GL/glew.h> is included before <GL/gl.h>, otherwise we
// get many errors
#include <GL/glew.h>
#include <GL/gl.h>
// clang-format on
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

// OpenGL works in 3D, but we want a 2D triangle, therefore our z axis for each
// vertex is 0
static float TRIANGLE_VERTICES[] = {
    -0.5f, -0.5f, 0.0f, //
    0.5f,  -0.5f, 0.0f, //
    0.0f,  0.5f,  0.0f, //
};

int main(int argc, char **argv) {
  if (uint32_t err = SDL_Init(SDL_INIT_VIDEO); err < 0) {
    std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Window *window = SDL_CreateWindow("Hello, world!", 0, 0, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  assert(window);

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (GLenum err = glewInit(); err != GLEW_OK) {
    std::cerr << "Unable to initialize GLEW: " << glewGetErrorString(err)
              << std::endl;
    return 1;
  }

  // Vertex buffer object (VBO) is an OpenGL object Basically, we create an
  // VBO, allocate memory on our GPU and it is used for storing our vertices.

  // GLuint vertex_buffer_object_id;
  // glGenBuffers(1, &vertex_buffer_object_id);
  // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_id);

  // Copying triangle vertices to the GPU's memory The last argument means we
  // want to send the data once and use it many times Therefore, it is a static
  // draw, however we have other types, such as GL_DYNAMIC_DRAW que fala que
  // o dado será usado e alterado várias vezes.

  // glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_VERTICES), TRIANGLE_VERTICES,
  //              GL_STATIC_DRAW);

  int32_t running = 1;
  int32_t fullscreen = 0;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;
        case 'f':
          fullscreen = !fullscreen;
          SDL_SetWindowFullscreen(window,
                                  fullscreen ? SDL_WINDOW_OPENGL |
                                                   SDL_WINDOW_FULLSCREEN_DESKTOP
                                             : SDL_WINDOW_OPENGL);
          break;
        default:
          break;
        }
      } else if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    // Resize screen
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Clear screen and set some color
    glClearColor(.5f, .5f, .5f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update window with OpenGL rendering
    SDL_GL_SwapWindow(window);
  }
  return 0;
}
