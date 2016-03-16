#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;

void main() {
  float depth = texture(frameTexture, fragUv).r;
  color = vec4(vec3(depth), 1.0f);

  //vec3 sample = vec3(texture(frameTexture, fragUv));
  //color = vec4(sample, 1.0f);
}
