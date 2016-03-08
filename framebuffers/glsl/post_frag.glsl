#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;

// The offset may or may not be representative of an actual pixel.
const float offset = 1.0 / 600;

void main() {
  // The precomputed offsets for a 3x3 kernel.
  vec2 offsets[9] = vec2[](
    vec2(-offset, offset),  // top-left
    vec2(0.0f,    offset),  // top-center
    vec2(offset,  offset),  // top-right
    vec2(-offset, 0.0f),    // center-left
    vec2(0.0f,    0.0f),    // center-center
    vec2(offset,  0.0f),    // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f,    -offset), // bottom-center
    vec2(offset,  -offset)  // bottom-right
  );

  float kernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
  );

  // Execute the kernel.
  vec3 col = vec3(0.0f);
  for (int i = 0; i < 9; i++) {
    vec2 coord = fragUv.st + offsets[i];
    vec3 sample = vec3(texture(frameTexture, coord));
    col += sample * kernel[i];
  }

  color = vec4(col, 1.0f);
}
