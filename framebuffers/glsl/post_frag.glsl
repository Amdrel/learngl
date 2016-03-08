#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;
uniform float time;

// The offset may or may not be representative of an actual pixel.
const float offset = 1.0 / 600;

void main() {
  float wave = sin(time + floor(fragUv.t * 35) / 35) / 2;

  vec2 coord = vec2(fragUv.s + wave / 2, fragUv.t);
  vec3 sample = vec3(texture(frameTexture, coord));
  color = vec4(sample, 1.0f);
}
