#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 fragColor;
out vec2 fragTexCoord;

void main() {
  fragColor = color;

  // Flip the texcoord on the y axis since the image loader loads them upside
  // down for mystical reasons.
  fragTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);

  gl_Position = vec4(position, 1.0);
}
