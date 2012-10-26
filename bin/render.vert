#version 140

layout (location = 0) in vec2 texture;

uniform sampler2D sampler;

void main(void) {
  gl_Position = vec4(2.f, 2.f, 0.f, 1.f) * (vec4(texture2D(sampler, texture).xyz, 1.f) - vec4(.5f, .5f, 0.f, 0.f));
}
