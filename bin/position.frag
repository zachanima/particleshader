#version 140

in vec2 vertexTexture;

out vec4 color;

uniform sampler2D previousSampler;
uniform sampler2D currentSampler;

void main(void) {
  vec4 previous = texture2D(previousSampler, vertexTexture);
  vec4 current = texture2D(currentSampler, vertexTexture);
  vec4 acceleration = vec4(0.f, 0.f, 0.f, 0.f);

  for (float y = 0.f; y < 1.f; y += 1.f / 128.f) {
    for (float x = 0.f; x < 1.f; x += 1.f / 128.f) {
      vec4 body = texture2D(currentSampler, vec2(x, y));
      const vec3 R = body.xyz - current.xyz;
      float dist = max(distance(body.xyz, current.xyz), .025f);
      const float INV_3_DIST = 1.f / sqrt(pow(dist, 6));
      const float S = INV_3_DIST;
      acceleration += vec4(R * S * .00000001f, 0.f);
    }
  }

  color = 2.f * current - previous + acceleration;

  if (color.x > 1.f || color.x < -1.f) {
    color.x = current.x;
  }
  if (color.y > 1.f || color.y < -1.f) {
    color.y = current.y;
  }
}
