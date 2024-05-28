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

// TODO: error handling during shader compilation
static GLuint compile_shader(GLuint type, const std::string &source) {
  GLuint shader_id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader_id, 1, &src, nullptr);
  glCompileShader(shader_id);

  GLint status;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLint length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(shader_id, length, &length, message);
    std::cerr << "fail to compile shader: " << message << std::endl;
  }
  return shader_id;
}

static int create_shader(const std::string &vertex_shader_source,
                         const std::string &fragment_shader_source) {
  GLuint program = glCreateProgram();

  GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
  GLuint fragment_shader =
      compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

static float TRIANGLE[] = {
    -0.5f, -0.5f, //
    0.5,   -0.5f, //
    0.5f,  0.5f,  //
    -0.5,  0.5f,  //
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

  int32_t running = 1;
  int32_t fullscreen = 0;

  // Index buffer for telling OpenGL how to render the square without
  // providing repeated buffer
  GLuint index_buffer[] = {
      0, 1, 2, // First triangle indices
      2, 3, 0, // Second triangle indices
  };

  GLuint index_buffer_object;
  // Generate a buffer with id 1
  glGenBuffers(1, &index_buffer_object);
  // Say 'buffer' is an vertex buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
  // Store the vertex buffer in the GPU
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer,
               GL_STATIC_DRAW);

  // Buffer -> Shader -> Draw
  //
  // Buffer - Buffer that will be drawn
  // Our vertex buffer
  GLuint buffer;
  // Generate a buffer with id 2
  glGenBuffers(2, &buffer);
  // Say 'buffer' is an vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  // Store the vertex buffer in the GPU
  glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE), TRIANGLE, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  // Define the memory layout of our vertex buffer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  // If I run this code without the shader, this code will probably run
  // because some GPUs actually provides default shaders and you can see
  // a triangle on the screen.

  // Shader - How the buffer will be drawn

  std::string vertex_shader_program =
      "#version 330 core\n"
      // (location = 0) is basically our vertex attribute index
      "layout (location = 0) in vec4 position;\n"
      "void main() {\n"
      "    gl_Position = position;\n"
      "}\n";

  std::string fragment_shader_program =
      "#version 330 core\n"
      "layout (location = 0) out vec4 color;\n"
      "void main() {\n"
      "    color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
      "}\n";

  GLuint shader = create_shader(vertex_shader_program, fragment_shader_program);
  glUseProgram(shader);

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

    // It uses the list of indices for drawing both triangles efficiently
    // instead of passing repeated positions and waste memory.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Resize screen
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Update window with OpenGL rendering
    SDL_GL_SwapWindow(window);
  }

  glDeleteProgram(shader);
  return 0;
}
