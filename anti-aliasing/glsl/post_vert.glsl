#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 fragUv;

void main() {
  fragUv = uv;

  // We're working in 2D so ignore the z and w components. There are no
  // transformations applied as the passed quad is meant to fill the clip space.
  gl_Position = vec4(position.xy, 0.0f, 1.0f);
}
