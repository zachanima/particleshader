#include "system.hpp"

GLuint System::ibo;
GLuint System::vbo;
GLuint System::quadfbo;
GLuint System::quadibo;
GLuint System::quadvbo;
GLuint System::positionTexture[3];
GLuint System::positionProgram;
GLuint System::renderProgram;
GLushort System::swap = 0;

GLvoid System::initialize() {
  const GLushort quadis[] = { 0, 1, 2, 3 };
  const GLfloat quadvs[] = {
    -1.f,  1.f, 0.f, 0.f, 1.f,
    -1.f, -1.f, 0.f, 0.f, 0.f,
     1.f,  1.f, 0.f, 1.f, 1.f,
     1.f, -1.f, 0.f, 1.f, 0.f
  };
  static GLfloat data[PARTICLES * 4];
  GLuint is[VERTICES];
  GLfloat vs[VERTICES * 3];
  GLuint samplerUniform;
  GLuint previousSamplerUniform;
  GLuint currentSamplerUniform;

  // Initialize textured quad framebuffer object.
  glGenFramebuffersEXT(1, &quadfbo);

  // Initialize textured quad index buffer object.
  glGenBuffers(1, &quadibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), quadis, GL_STATIC_DRAW);

  // Initialize textured quad vertex buffer object.
  glGenBuffers(1, &quadvbo);
  glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), quadvs, GL_STATIC_DRAW);

  // Initialize position previous/current/next textures.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(3, positionTexture);
  glBindTexture(GL_TEXTURE_2D, positionTexture[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  for (size_t i = 0; i < PARTICLES * 4; i += 4) {
    data[i+0] = .5f * (GLfloat)rand() / (GLfloat)RAND_MAX - .25f;
    data[i+1] = .5f * (GLfloat)rand() / (GLfloat)RAND_MAX - .25f;
    data[i+2] = .5f * (GLfloat)rand() / (GLfloat)RAND_MAX - .25f;
    data[i+3] = .5f * (GLfloat)rand() / (GLfloat)RAND_MAX - .25f;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SQRT_PARTICLES, SQRT_PARTICLES, 0, GL_RGBA, GL_FLOAT, data);

  glBindTexture(GL_TEXTURE_2D, positionTexture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SQRT_PARTICLES, SQRT_PARTICLES, 0, GL_RGBA, GL_FLOAT, data);

  glBindTexture(GL_TEXTURE_2D, positionTexture[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SQRT_PARTICLES, SQRT_PARTICLES, 0, GL_RGBA, GL_FLOAT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize particle index buffer object.
  for (size_t i = 0; i < VERTICES; i++) {
    is[i] = i;
  }
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICES * sizeof(GLuint), is, GL_STATIC_DRAW);

  // Initialize particle vertex buffer object.
  size_t i = 0;
  for (float y = 0.f; y < 1.f; y += 1.f / (GLfloat)SQRT_PARTICLES) {
    for (float x = 0.f; x < 1.f; x += 1.f / (GLfloat)SQRT_PARTICLES) {
      vs[i++] = x;
      vs[i++] = y;
      vs[i++] = 0.f;
    }
  }
  for (float y = 0.f; y < 1.f; y += 1.f / (GLfloat)SQRT_PARTICLES) {
    for (float x = 0.f; x < 1.f; x += 1.f / (GLfloat)SQRT_PARTICLES) {
      vs[i++] = x;
      vs[i++] = y;
      vs[i++] = 1.f;
    }
  }
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * 3 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize position shaders.
  positionProgram = Display::shaders("position.vert", "position.frag");
  previousSamplerUniform = glGetUniformLocation(positionProgram, "previousSampler");
  currentSamplerUniform = glGetUniformLocation(positionProgram, "currentSampler");
  glUseProgram(positionProgram);
  glUniform1i(previousSamplerUniform, 0);
  glUniform1i(currentSamplerUniform, 1);
  glUseProgram(0);

  // Initialize render shaders.
  renderProgram = Display::shaders("render.vert", "render.frag");
  samplerUniform = glGetUniformLocation(renderProgram, "sampler");
  glUseProgram(renderProgram);
  glUniform1i(samplerUniform, 0);
  glUseProgram(0);

  // Initialize point size, enable blending.
  glPointSize(1.f);
  glEnable(GL_POINT_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



GLvoid System::update() {
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  const GLuint previous = swap;
  const GLuint current = (swap + 1) % 3;
  const GLuint next = (swap + 2) % 3;

  // Initialize framebuffer object, attach next texture.
  glBindFramebufferEXT(GL_FRAMEBUFFER, quadfbo);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture[next], 0);
  glDrawBuffers(1, buffers);

  // Render new positions to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glViewport(0, 0, SQRT_PARTICLES, SQRT_PARTICLES);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(positionProgram);
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, positionTexture[previous]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, positionTexture[current]);
  glActiveTexture(GL_TEXTURE0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadibo);
  glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
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
  glPopAttrib();

  glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

  swap++;
  swap %= 3;
}



GLvoid System::render() {
  const GLuint current = (swap + 1) % 3;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);

  glUseProgram(renderProgram);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, positionTexture[current]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

  glDrawElements(GL_POINTS, VERTICES, GL_UNSIGNED_INT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);

  glDisable(GL_BLEND);
}
