#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;
uniform float time;

void main() {
  vec3 sample = vec3(texture(frameTexture, fragUv));

  color = vec4(sample, 1.0f);

  if (gl_FragCoord.x < 400) {
    color *= vec4(vec3(0.8f, 0.8f, 1.0f), 1.0f);
  } else {
    color *= vec4(vec3(1.0f, 0.8f, 0.8f), 1.0f);
  }
}
