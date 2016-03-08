#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;

void main() {
  vec4 sample = texture(frameTexture, fragUv);
  float average = 0.2126 * sample.r + 0.7152 * sample.g + 0.0722 * sample.b;
  color = vec4(vec3(average), 1.0f);
}
