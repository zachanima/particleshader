#version 140

layout (location = 0) in vec3 texture;

uniform sampler2D sampler;

void main(void) {
  vec2 body;
  if (texture.z == 0.f) {
    body = texture2D(sampler, texture.xy).xy;
  } else {
    body = texture2D(sampler, texture.xy).zw;
  }
  gl_Position = vec4(body * 0.25f, 0.f, 1.f);
}
