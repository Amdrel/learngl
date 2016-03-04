#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

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

  // Calculate specular lighting for the fragment based on the view position.
  float specularStrength = 0.5f;
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 2);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}
