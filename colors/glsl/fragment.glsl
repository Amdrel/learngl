#version 330 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
  float ambientStrength = 0.1f;
  vec3 ambient = lightColor * ambientStrength;

  vec3 result = objectColor * ambient;
  color = vec4(result, 1.0f);
}
