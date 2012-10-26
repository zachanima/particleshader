#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

void main(void) {
  color = texture2D(sampler, vertexTexture);
}
