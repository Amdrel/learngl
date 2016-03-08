#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;

void main() {
  vec4 sample = texture(frameTexture, fragUv);
  float average = (sample.x + sample.y + sample.z) / 3;

  vec3 inversion = vec3(1.0f - sample);
  color = vec4(inversion * average, 1.0f);
}
