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
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    // This method only exists in very early GL versions
    // Probably not even used anymore (don't know)
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    // Resize screen
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Update window with OpenGL rendering
    SDL_GL_SwapWindow(window);
  }
  return 0;
}
