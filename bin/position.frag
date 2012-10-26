#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

void main(void) {
  color = texture2D(sampler, vertexTexture) + vec4(0.f, .001f, 0.f, 0.f);
  if (color.y > 1.f) {
    color.y -= 1.f;
  }
}
