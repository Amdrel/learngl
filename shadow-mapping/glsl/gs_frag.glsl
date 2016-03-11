#version 330 core

in GS_OUT {
  vec3 color;
} frag_in;

out vec4 color;

void main() {
  color = vec4(frag_in.color, 1.0f);
}
