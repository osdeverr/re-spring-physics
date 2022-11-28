#version 410 core

in vec3 a_position;
in vec3 a_normal;
in int a_transform;
in int a_material;

uniform mat4 u_camera_matrix;
uniform samplerBuffer u_matrix_buffer;
uniform samplerBuffer u_material_buffer;

out float is_grid;
out vec3 vertex_position;
out vec3 vertex_color;
out vec4 screen_position;

void main() {
  int matrix_offset = a_transform * 4;

  mat4 object_transform = mat4(
    texelFetch(u_matrix_buffer, matrix_offset),
    texelFetch(u_matrix_buffer, matrix_offset + 1),
    texelFetch(u_matrix_buffer, matrix_offset + 2),
    texelFetch(u_matrix_buffer, matrix_offset + 3)
  );

  vec4 world_position = object_transform * vec4(a_position, 1);

  gl_Position = u_camera_matrix * world_position;
  screen_position = gl_Position;

  int material_offset = a_material * 2;
  vec4 material_data_a = texelFetch(u_material_buffer, material_offset);

  is_grid = material_data_a.a;
  vertex_color = material_data_a.rgb;

  vertex_position = world_position.xyz;
}
