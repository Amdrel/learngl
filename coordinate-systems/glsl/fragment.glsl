#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  vec4 mixed = mix(texture(texture1, fragTexCoord), texture(texture2, fragTexCoord), 0.2);
  color = mixed * vec4(fragColor, 1.0);
}
