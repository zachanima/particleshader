#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define SQRT_PARTICLES (16)
#define PARTICLES (SQRT_PARTICLES * SQRT_PARTICLES)

#define GL_GLEXT_PROTOTYPES

#include <cstdlib>

#include <GL/gl.h>
#include <GL/glext.h>

#include "lwcppgl/display.hpp"

class System {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint quadibo;
    static GLuint quadvbo;
    static GLuint positionTexture[2];
    static GLuint renderTexture;
    static GLuint positionProgram;
    static GLuint renderProgram;
    static GLushort swap;
};

#endif // SYSTEM_HPP
