#version 410 core

in vec3 a_position;
in vec3 a_normal;
in int a_transform;
in int a_material;

uniform mat4 u_camera_matrix;
uniform mat4 u_light_camera_matrix;
uniform samplerBuffer u_matrix_buffer;
uniform samplerBuffer u_material_buffer;

out float specular_factor;
out vec4 light_vertex_position;
out vec4 screen_position;
out vec3 vertex_normal;
out vec3 vertex_position;

void main() {
  int matrix_offset = a_transform * 4;
  mat4 object_transform = mat4(
    texelFetch(u_matrix_buffer, matrix_offset),
    texelFetch(u_matrix_buffer, matrix_offset + 1),
    texelFetch(u_matrix_buffer, matrix_offset + 2),
    texelFetch(u_matrix_buffer, matrix_offset + 3)
  );

  int material_offset = a_material * 2;
  vec4 material_data_b = texelFetch(u_material_buffer, material_offset + 1);
  specular_factor = material_data_b.r;

  vec4 world_position = object_transform * vec4(a_position, 1);

  light_vertex_position = u_light_camera_matrix  * world_position;
  gl_Position           = u_camera_matrix        * world_position;

  screen_position = gl_Position;
  vertex_normal = (object_transform * vec4(a_normal, 0)).xyz;
  vertex_position = world_position.xyz;
}
