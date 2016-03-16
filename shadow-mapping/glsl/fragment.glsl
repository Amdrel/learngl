#version 330 core

// Why isn't this available in GLSL in the first place?
#define PI 3.14159265

// Interpolated mesh data.
in GS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
  vec4 lightSpacePos;
} frag_in;

out vec4 color; // Final fragment color.

uniform vec3 lightPos;
uniform vec3 viewPos; // Used for specular calculation.

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap; // The shadow map for shadows.

float calcShadow(vec4 lightSpacePos, vec3 normal, vec3 lightDir) {
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  projCoords = projCoords * 0.5f + 0.5f;

  if (projCoords.z > 1.0f) {
    return 0.0f;
  }

  float closestDepth = texture(shadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;

  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0;
  return shadow;
}

void main() {
  vec3 sample = texture(diffuseTexture, frag_in.uv).rgb;
  vec3 normal = normalize(frag_in.normal);
  vec3 lightColor = vec3(1.0);

  // Ambient
  vec3 ambient = 0.15 * sample;

  // Diffuse
  vec3 lightDir = normalize(lightPos - frag_in.position);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular
  vec3 viewDir = normalize(viewPos - frag_in.position);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = spec * lightColor;

  // Calculate shadow
  float shadow = calcShadow(frag_in.lightSpacePos, normal, lightDir);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * sample;

  color = vec4(lighting, 1.0f);
}
