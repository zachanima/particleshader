#version 140

layout (location = 0) in vec2 texture;

uniform sampler2D sampler;

void main(void) {
  vec4 body = texture2D(sampler, texture);
  gl_Position = vec4(body.xy, 0.f, 1.f);
}
