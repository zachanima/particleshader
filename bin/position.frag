#version 140

in vec2 vertexTexture;

out vec4 color;

uniform sampler2D previousSampler;
uniform sampler2D currentSampler;

void main(void) {
  vec4 previous = texture2D(previousSampler, vertexTexture);
  vec4 current = texture2D(currentSampler, vertexTexture);
  vec4 acceleration = vec4(0.f, 0.f, 0.f, 0.f);

  for (float y = 0.f; y < 1.f; y += 1.f / 112.f) {
    for (float x = 0.f; x < 1.f; x += 1.f / 112.f) {
      const vec4 body = texture2D(currentSampler, vec2(x, y));
      const vec4 R0 = body - current;
      const vec4 R1 = body.zwxy - current;
      const vec4 R02 = R0 * R0;
      const vec4 R12 = R1 * R1;
      const vec4 Radd = vec4(R02.xz, R12.xz) + vec4(R02.yw, R12.yw);
      const vec4 dist = vec4(
        max(sqrt(Radd.x), .025f),
        max(sqrt(Radd.z), .025f),
        max(sqrt(Radd.y), .025f),
        max(sqrt(Radd.w), .025f)
      );
      const vec4 dist3 = dist * dist * dist;
      acceleration += vec4(
        .000000005f * (R0.xy / dist3.x + R1.xy / dist3.y),
        .000000005f * (R0.zw / dist3.z + R1.zw / dist3.w)
      );
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
