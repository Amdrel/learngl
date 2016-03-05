#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};
struct Light {
  // Coordinate info.
  vec3 position;
  vec3 direction;

  // Colors.
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // Attenuation.
  float constant;
  float linear;
  float quadratic;

  // Spot light values.
  float cutoff;
  float outerCutoff;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUv;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 emissionColor;

void main() {
  // Calculate the ambient light value for the fragment.
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUv));

  // Calculate diffuse lighting for the fragment using the passed light source
  // in the uniform.
  // TODO: Can light positions be passed as textures or in vertex attributes?
  vec3 lightDirection = normalize(light.position - fragPos);
  vec3 normal = normalize(fragNormal);
  float diff = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUv));

  // Calculate specular lighting for the fragment based on the view position.
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUv));

  // Calculate cutoff.
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
  diffuse *= intensity;
  specular *= intensity;

  // Calculate attenuation for light intensity falloff.
  float lightDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance +
    light.quadratic * (lightDistance * lightDistance));
  diffuse  *= attenuation;
  specular *= attenuation;

  // Sample the emission map for magic glows.
  vec3 emission = vec3(texture(material.emission, fragUv)) * emissionColor;
  emission *= intensity * attenuation;

  color = vec4(ambient + diffuse + specular + emission, 1.0f);
}
