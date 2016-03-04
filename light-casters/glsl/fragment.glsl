#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
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
in vec2 fragUv;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
  // Calculate ambient light for the fragment.
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUv));

  // Calculate diffuse lighting for the fragment using the passed light source
  // in the uniform.
  // TODO: Can light positions be passed as textures or in vertex attributes?
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUv));

  // Calculate specular lighting for the fragment based on the view position.
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUv));

  // Sample the emission map for magic glows.
  vec3 emission = vec3(texture(material.emission, fragUv));

  color = vec4(ambient + diffuse + specular + emission, 1.0f);
}
