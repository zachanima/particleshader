#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

void main(void) {
  color = vec4(0.f, 0.f, 0.f, 1.f);

  // Determine brightness of this pixel based on nearby elements.
  for (float y = 0; y < 1.f; y += 1.f / 8.f) {
    for (float x = 0; x < 1.f; x += 1.f / 8.f) {
      vec3 xyz = texture2D(sampler, vec2(x, y)).xyz;
      float dist = distance(xyz, vec3(vertexTexture, xyz.z));
      dist = 1.f / pow(dist, 2) * 0.00001f;
      color += vec4(dist, dist, dist, 0.f);
    }
  }
}
