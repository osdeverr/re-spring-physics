#version 410 core

in vec3 a_position;
in vec3 a_normal;
in int a_transform;
in int a_material;

uniform mat4 u_light_camera_matrix;
uniform samplerBuffer u_matrix_buffer;

out float vertex_depth;

void main() {
  int matrix_offset = a_transform * 4;
  mat4 object_transform = mat4(
    texelFetch(u_matrix_buffer, matrix_offset),
    texelFetch(u_matrix_buffer, matrix_offset + 1),
    texelFetch(u_matrix_buffer, matrix_offset + 2),
    texelFetch(u_matrix_buffer, matrix_offset + 3)
  );

  vec4 world_position = object_transform * vec4(a_position, 1);

  gl_Position  = u_light_camera_matrix * world_position;
  vertex_depth = gl_Position.z;
}
