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
in GS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} frag_in;

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

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  // Calculate prerequisites for the light calculations.
  vec3 lightDir = normalize(-light.direction);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  // Calculate the light multipliers using the light formulae.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_in.uv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_in.uv));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_in.uv));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
  // Calculate prerequisites for the light calculations.
  vec3 lightDir = normalize(light.position - frag_in.position);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  // Calculate light multipliers.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_in.uv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_in.uv));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_in.uv));

  // Calculate attenuation for light intensity falloff.
  float lightDistance = length(light.position - frag_in.position);
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
  vec3 lightDir = normalize(light.position - frag_in.position);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  // Calculate light multipliers.
  float diff = max(dot(normal, lightDir), 0.0f);
  float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

  // Sample the material's diffuse and multiply the colors and the light values.
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_in.uv));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_in.uv));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_in.uv));

  // Calculate cutoff.
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
  ambient  *= intensity;
  diffuse  *= intensity;
  specular *= intensity;

  // Calculate attenuation for light intensity falloff.
  float lightDistance = length(light.position - frag_in.position);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance +
    light.quadratic * (lightDistance * lightDistance));

  // Apply the attenuation to all light calculations.
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  // Sample the emission map for magic glows.
  vec3 emission = vec3(texture(material.emission, frag_in.uv));
  emission *= intensity * attenuation; // Intensity for a lens of truth effect.

  return (ambient + diffuse + specular + emission);
}

void main() {
  vec3 normal = normalize(frag_in.normal);
  vec3 viewDir = normalize(viewPos - frag_in.position);

  // Calculate the directional light value.
  vec3 result = calcDirectionalLight(dirLight, normal, viewDir);

  // Add all the point light values to the result.
  for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
    result += calcPointLight(pointLights[i], normal, viewDir);
  }

  // Add all the spot light values to the result.
  //for (int i = 0; i < SPOT_LIGHT_COUNT; i++) {
  //  result += calcSpotLight(spotLights[i], normal, viewDir);
  //}

  color = vec4(result, 1.0f);
}
