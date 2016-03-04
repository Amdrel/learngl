#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
  // Calculate ambient light for the fragment.
  vec3 ambient = light.ambient * material.ambient;

  // Calculate diffuse lighting for the fragment using the passed light source
  // in the uniform.
  // TODO: Can light positions be passed as textures or in vertex attributes?
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  // Calculate specular lighting for the fragment based on the view position.
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0f);
}
