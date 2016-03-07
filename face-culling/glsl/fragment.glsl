#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};
struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};
struct SpotLight {
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float cutoff;
  float outerCutoff;
};

// Interpolated mesh data.
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUv;

out vec4 color; // Final fragment color.

// Material chosen for the object.
uniform Material material;

#define POINT_LIGHT_COUNT 4
#define SPOT_LIGHT_COUNT 1

// Lights of all kinds.
uniform DirectionalLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];

uniform vec3 viewPos; // Used for specular calculation.
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  // Calculate prerequisites for the light calculations.
  vec3 lightDir = normalize(-light.direction);
  vec3 reflectDir = reflect(-lightDir, normal);

  // Calculate the light multipliers using the light formulae.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, fragUv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, fragUv));
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fragUv));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
  // Calculate prerequisites for the light calculations.
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);

  // Calculate light multipliers.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, fragUv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, fragUv));
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fragUv));

  // Calculate attenuation for light intensity falloff.
  float lightDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance +
    light.quadratic * (lightDistance * lightDistance));

  // Apply the attenuation to all light calculations.
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
  // Calculate prerequisites for the light calculations.
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);

  // Calculate light multipliers.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, fragUv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, fragUv));
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fragUv));

  // Calculate cutoff.
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
  ambient  *= intensity;
  diffuse  *= intensity;
  specular *= intensity;

  // Calculate attenuation for light intensity falloff.
  float lightDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance +
    light.quadratic * (lightDistance * lightDistance));

  // Apply the attenuation to all light calculations.
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  // Sample the emission map for magic glows.
  vec3 emission = vec3(texture(material.emission, fragUv));
  emission *= intensity * attenuation; // Intensity for a lens of truth effect.

  return (ambient + diffuse + specular + emission);
}

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 viewDir = normalize(viewPos - fragPos);

  // Calculate the directional light value.
  vec3 result = calcDirectionalLight(dirLight, normal, viewDir);

  // Add all the point light values to the result.
  for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
    result += calcPointLight(pointLights[i], normal, viewDir);
  }

  // Add all the spot light values to the result.
  for (int i = 0; i < SPOT_LIGHT_COUNT; i++) {
    result += calcSpotLight(spotLights[i], normal, viewDir);
  }

  vec3 edgeColor = vec3(0.05f);
  float edge = max(dot(normal, viewDir), 0.0f);

  if (edge <= 0.2f) {
    result *= edgeColor;
  }

  color = vec4(result, 1.0f);
}
