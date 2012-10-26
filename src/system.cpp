#include "system.hpp"

GLuint System::ibo;
GLuint System::vbo;
GLuint System::positionTexture[2];
GLuint System::positionProgram;
GLuint System::renderProgram;
GLushort System::swap = 0;

GLvoid System::initialize() {
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f, 0.f, 0.f, 1.f,
    -1.f, -1.f, 0.f, 0.f, 0.f,
     1.f,  1.f, 0.f, 1.f, 1.f,
     1.f, -1.f, 0.f, 1.f, 0.f
  };
  GLfloat *data = new GLfloat[8 * 8 * 4];
  GLuint samplerUniform;

  // Initialize pixel data.
  for (size_t i = 0; i < 8 * 8 * 4; i += 4) {
    data[i+0] = (GLfloat)rand() / (GLfloat)RAND_MAX;
    data[i+1] = (GLfloat)rand() / (GLfloat)RAND_MAX;
    data[i+2] = (GLfloat)rand() / (GLfloat)RAND_MAX;
    data[i+3] = 1.f;
  }

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize position texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &positionTexture[swap]);
  glBindTexture(GL_TEXTURE_2D, positionTexture[swap]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_FLOAT, data);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize position shaders.
  // positionProgram = Display::shaders("position.vert", "position.frag");

  // Initialize render shaders.
  renderProgram = Display::shaders("render.vert", "render.frag");
  samplerUniform = glGetUniformLocation(renderProgram, "sampler");
  glUseProgram(renderProgram);
  glUniform1i(samplerUniform, 0);
  glUseProgram(0);

  // Delete pixel data.
  delete data;
}



GLvoid System::update() {
  
}



GLvoid System::render() {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(renderProgram);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, positionTexture[swap]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)12);

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}
