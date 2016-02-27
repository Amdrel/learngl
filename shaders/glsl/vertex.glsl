#version 330 core

layout (location = 0) in vec3 position; // The position is in position 0.
layout (location = 1) in vec3 color;    // The color is in position 1.

out vec3 ourColor;

void main() {
  gl_Position = vec4(position, 1.0);
  ourColor = color;
}
