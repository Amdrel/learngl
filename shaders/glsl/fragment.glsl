#version 330 core

in vec3 ourColor; // Color passed from the vertex shader.
out vec4 color;   // Color output for the pixel.

uniform vec4 timeColor;

void main() {
  color = vec4(ourColor, 1.0f) * timeColor;
}
