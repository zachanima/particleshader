#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "lwcppgl/display.hpp"

class System {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint ibo;
    static GLuint vbo;
    static GLuint positionTexture;
    static GLuint positionProgram;
};

#endif // SYSTEM_HPP
