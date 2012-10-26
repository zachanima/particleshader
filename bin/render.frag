#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;
uniform int sqrtParticles;

void main(void) {
  color = vec4(0.f, 0.f, 0.f, 1.f);

  // Determine brightness of this pixel based on nearby stars.
  for (float y = 0; y < 1.f; y += 1.f / sqrtParticles) {
    for (float x = 0; x < 1.f; x += 1.f / sqrtParticles) {
      vec3 xyz = texture2D(sampler, vec2(x, y)).xyz;
      float dist = distance(xyz, vec3(vertexTexture, 0.f));
      dist = 1.f / pow(dist, 2) * 0.000001f;
      color += vec4(dist, dist, dist, 0.f);
    }
  }
}
