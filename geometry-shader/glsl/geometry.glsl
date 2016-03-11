#version 330 core

#define VERTEX_COUNT 3

layout (triangles) in;
layout (triangle_strip, max_vertices = VERTEX_COUNT) out;

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} gs_in[];

out GS_OUT {
  vec3 position;
  vec3 normal;
  vec2 uv;
} gs_out;

uniform float time;

vec4 explode(vec4 position, vec3 normal, float magnitude) {
  vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
  return position + vec4(direction, 0.0f);
}

void main() {
  for (int i = 0; i < VERTEX_COUNT; i++) {
    //gs_out.position = gs_in[i].position + vec3((sin(time) + 1.0f) / 16.0f);
    gs_out.position = gs_in[i].position;
    gs_out.normal = gs_in[i].normal;
    gs_out.uv = gs_in[i].uv;
    //gl_Position = explode(gl_in[i].gl_Position, gs_in[i].normal, 1.0f);
    gl_Position = gl_in[i].gl_Position;
    //gl_Position = gl_in[i].gl_Position + vec4(vec3((sin(time) + 1.0f) / 16.0f), 0.0f);
    EmitVertex();
  }

  EndPrimitive();
}
