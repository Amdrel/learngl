#version 330 core

in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  // Mix the 2 textures and become one!
  vec4 mixed = mix(texture(texture1, fragTexCoord), texture(texture2, fragTexCoord), 0.2);

  // Set the fragment to the final color.
  color = mixed;
}
