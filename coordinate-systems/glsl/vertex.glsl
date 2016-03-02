#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // Flip the texcoord on the y axis since the image loader loads them upside
  // down for mystical reasons.
  fragTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);

  // Apply the object's transform to the vertex.
  gl_Position = projection * view * model * vec4(position, 1.0);
}
