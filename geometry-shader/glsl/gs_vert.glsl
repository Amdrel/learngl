#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

// Color data to send to the geometry shader.
out VS_OUT {
  vec3 color;
} vs_out;

void main() {
  gl_Position = vec4(position.xy, 0.0f, 1.0f);
  vs_out.color = color;
}
