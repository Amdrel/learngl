#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Stripped version of the model matrix without the translation information.
uniform mat3 normalMatrix;

void main() {
  // Get the fragment position by getting the vertex position in world space
  // and letting OpenGL interpolate it (since we are using out).
  fragPos = vec3(model * vec4(position, 1.0f));
  fragNormal = normalMatrix * normal;
  fragUv = vec2(uv.x, 1.0f - uv.y);

  // Apply the object's transform to the vertex.
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
