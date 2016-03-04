#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
  // Calculate ambient light for the fragment.
  float ambientStrength = 0.1f;
  vec3 ambient = lightColor * ambientStrength;

  // Calculate diffuse lighting for the fragment using the passed light source
  // in the uniform.
  // TODO: Can light positions be passed as textures or in vertex attributes?
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(lightPos - fragPos);
  float diff = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse) * objectColor;
  color = vec4(result, 1.0f);
}
