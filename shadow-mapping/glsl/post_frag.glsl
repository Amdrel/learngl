#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;

void main() {
  vec3 sample = vec3(texture(frameTexture, fragUv));
  color = vec4(sample, 1.0f);
}
