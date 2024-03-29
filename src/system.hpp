#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define SQRT_PARTICLES (112)
#define PARTICLES (SQRT_PARTICLES * SQRT_PARTICLES)
#define VERTICES (PARTICLES * 2)

#include <cmath>
#include <cstdlib>

#include <GL/glew.h>

#include "lwcppgl/display.hpp"

class System {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint ibo;
    static GLuint vbo;
    static GLuint quadfbo;
    static GLuint quadibo;
    static GLuint quadvbo;
    static GLuint positionTexture[3];
    static GLuint positionProgram;
    static GLuint renderProgram;
    static GLushort swap;
};

#endif // SYSTEM_HPP
