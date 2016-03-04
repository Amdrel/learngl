#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // Get the fragment position by getting the vertex position in world space
  // and letting OpenGL interpolate it (since we are using out).
  fragPos = vec3(model * vec4(position, 1.0f));
  fragNormal = normal;

  // Apply the object's transform to the vertex.
  gl_Position = projection * view * model * vec4(position, 1.0f);
}
